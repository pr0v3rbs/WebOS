#include <linux/kernel.h>

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>

struct clk_lg1313 {
	struct clk_hw hw;
	void __iomem *base[2];
	u32 fix[3];
};

#define to_clk_lg1313(_hw)	container_of(_hw, struct clk_lg1313, hw)

#pragma pack(push, 1)

struct lg1313_pll_ctrl {
	u32	/* RESERVED */	:  1;
	u32	fine_div	:  1;
	u32	dss		:  1;
	u32	accuracy	:  2;
	u32	updn_max	:  7;
	u32	mod_freq	:  9;
	u32	m		:  6;
	u32	/* RESERVED */	:  1;
	u32	cih		:  4;

	u32	freq_update	:  1;
	u32	sync_on		:  1;
	u32	/* RESERVED */	:  2;
	u32	ofs_mode	:  1;
	u32	fout3_od	:  3;
	u32	fout2_od	:  3;
	u32	fout_od		:  2;
	u32	pre_fout3_div	:  3;
	u32	pre_fout2_div	:  3;
	u32	nsc		:  4;
	u32	npc		:  6;
	u32	fine_control	:  3;

	u32	/* RESERVED */	: 22;
	u32	wakeup_bypass	:  1;
	u32	ddr_mode	:  1;
	u32	offset		:  8;
};

#pragma pack(pop)

static unsigned long
clk_lg1313_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
	struct clk_lg1313 const *clk = to_clk_lg1313(hw);
	u32 regs[3];
	struct lg1313_pll_ctrl *ctrl = (struct lg1313_pll_ctrl *)regs;
	unsigned long rate;

	regs[0] = clk->fix[0] ^ readl_relaxed(clk->base[0] + 0x00);
	regs[1] = clk->fix[1] ^ readl_relaxed(clk->base[0] + 0x04);
	regs[2] = clk->fix[2] ^ readl_relaxed(clk->base[0] + 0x08);

	parent_rate /= 1000000;	/* in MHz */

	rate = ((ctrl->npc << 2) + ctrl->nsc) * parent_rate /
	       ((ctrl->m + 1) << ctrl->fout_od);

	return rate * 1000000;	/* in Hz */
}

static long
clk_lg1313_round_rate(struct clk_hw *hw, unsigned long rate,
                      unsigned long *parent_rate)
{
	unsigned long freq_in, freq_out, freq_vco;
	unsigned int feedback;
	int div;

	/* calculate in MHz */
	freq_in = *parent_rate / 1000000;
	freq_out = rate / 1000000;

	for (div = 3; div >= 0; div--) {
		freq_vco = freq_out << div;

		if (freq_vco < 1200 || freq_vco > 2700)
			continue;

		feedback = (freq_vco + (freq_in >> 1)) / freq_in;
		freq_out = (feedback * freq_in) >> div;

		return (long)(freq_out * 1000000);
	}

	return -ENOENT;
}

static int
__find_ctrl(struct lg1313_pll_ctrl *ctrl, unsigned long freq_out,
            unsigned long freq_in)
{
	unsigned int feedback;
	unsigned long freq_vco;
	unsigned int const cih_map[][2] = {
		{ 2650, 0b1100 }, { 2480, 0b1010 },
		{ 2300, 0b1001 }, { 2080, 0b0111 },
		{ 1870, 0b0110 }, { 1600, 0b0100 },
		{ 1200, 0b0011 },
	};
	unsigned int npc, nsc;
	int div, i;

	if (!ctrl)
		return -EINVAL;

	/* calculate in MHz */
	freq_in /= 1000000;
	freq_out /= 1000000;

	for (div = 3; div >= 0; div--) {
		freq_vco = freq_out << div;

		if (freq_vco % freq_in)
			continue;
		if (freq_vco < 1200 || freq_vco > 2700)
			continue;

		feedback = freq_vco / freq_in;
		npc = (feedback - 4) / 4;
		nsc = feedback - npc * 4;

		for (i = 0; i < ARRAY_SIZE(cih_map); i++) {
			if (freq_vco > cih_map[i][0]) {
				ctrl->cih = cih_map[i][1];
				ctrl->fout_od = div;
				ctrl->npc = npc;
				ctrl->nsc = nsc;

				return 0;
			}
		}
	}

	return -ENOENT;
}

static int clk_lg1313_set_rate(struct clk_hw *hw, unsigned long rate,
                               unsigned long parent_rate)
{
	struct clk_lg1313 const *clk = to_clk_lg1313(hw);
	u32 regs[3], v;
	struct lg1313_pll_ctrl *ctrl = (struct lg1313_pll_ctrl *)regs;
	int ret;

	/* read current control values */
	regs[0] = clk->fix[0] ^ readl_relaxed(clk->base[0] + 0x00);
	regs[1] = clk->fix[1] ^ readl_relaxed(clk->base[0] + 0x04);
	regs[2] = clk->fix[2] ^ readl_relaxed(clk->base[0] + 0x08);

	/* find correct control values for target freq. */
	ret = __find_ctrl(ctrl, rate, parent_rate);
	if (ret < 0) {
		pr_err("%s: can't find proper parameters\n", __func__);
		return ret;
	}

	/* switch to stable 600MHz clock */
	v = readl_relaxed(clk->base[1]) | 0x2000;
	writel_relaxed(v, clk->base[1]);

	/* commit new control values */
	writel_relaxed(regs[0] ^ clk->fix[0], clk->base[0] + 0x00);
	writel_relaxed(regs[1] ^ clk->fix[1], clk->base[0] + 0x04);
	writel_relaxed(regs[2] ^ clk->fix[2], clk->base[0] + 0x08);

	udelay(50);	/* stabilizing */

	/* switch back to stabilized PLL */
	v = readl_relaxed(clk->base[1]) ^ 0x2000;
	writel_relaxed(v, clk->base[1]);

	return 0;
}

static struct clk_ops const clk_lg1313_ops = {
	.recalc_rate	= clk_lg1313_recalc_rate,
	.round_rate	= clk_lg1313_round_rate,
	.set_rate	= clk_lg1313_set_rate,
};

static struct clk *
clk_register_lg1313(struct device *dev, char const *name,
                    char const *parent_name, unsigned long flags,
                    struct clk_ops const *ops, void __iomem *base[],
                    u32 const fix[])
{
	struct clk_lg1313 *clk_lg1313;
	struct clk_init_data init_data;
	struct clk *clk;

	clk_lg1313 = kzalloc(sizeof(struct clk_lg1313), GFP_KERNEL);
	if (!clk_lg1313) {
		pr_err("%s: can't allocate memory\n", __func__);
		return ERR_PTR(-ENOMEM);
	}

	init_data.name = name;
	init_data.ops = ops;
	init_data.flags = flags | CLK_IS_BASIC;
	init_data.parent_names = &parent_name;
	init_data.num_parents = 1;

	clk_lg1313->hw.init = &init_data;
	clk_lg1313->base[0] = base[0];
	clk_lg1313->base[1] = base[1];
	clk_lg1313->fix[0] = fix[0];
	clk_lg1313->fix[1] = fix[1];
	clk_lg1313->fix[2] = fix[2];

	clk = clk_register(dev, &clk_lg1313->hw);
	if (IS_ERR(clk))
		kfree(clk_lg1313);

	return clk;
}

static void __init clk_lg1313_of_setup(struct device_node *np)
{
	struct clk *clk;
	char const *clk_name = np->name;
	char const *parent_name;
	void __iomem *base[2];
	u32 fix[3];

	of_property_read_string(np, "clock-output-names", &clk_name);
	parent_name = of_clk_get_parent_name(np, 0);
	base[0] = of_iomap(np, 0);
	base[1] = of_iomap(np, 1);
	of_property_read_u32_array(np, "fix", fix, 3);

	if (!parent_name) {
		pr_err("%s: can't find parent clock name\n", __func__);
		return;
	}
	if (!base[0] || !base[1]) {
		pr_err("%s: can't find control address\n", __func__);
		return;
	}

	clk = clk_register_lg1313(NULL, clk_name, parent_name,
	                          CLK_GET_RATE_NOCACHE, &clk_lg1313_ops, base,
	                          fix);
	if (!IS_ERR(clk))
		of_clk_add_provider(np, of_clk_src_simple_get, clk);
}
CLK_OF_DECLARE(clk_lg1313, "lge,lg1313-clk", clk_lg1313_of_setup);
