# =============================================================================
# Preamble
# =============================================================================
        .arch armv7-a
        .fpu neon
        .text
        .align 4
        .global yc_separate_tile_neon_256
        .type   yc_separate_tile_neon_256, %function
        .global yc_separate_tile_neon_256_y_only
        .type   yc_separate_tile_neon_256_y_only %function
        .global yc_separate_tile_neon_128
        .type   yc_separate_tile_neon_128, %function
        .global yc_separate_tile_neon_128_y_only
        .type   yc_separate_tile_neon_128_y_only %function
        .global yc_separate_tile_neon_64
        .type   yc_separate_tile_neon_64, %function
        .global yc_separate_tile_neon_64_y_only
        .type   yc_separate_tile_neon_64_y_only %function
        .global yc_separate_tile_neon_32
        .type   yc_separate_tile_neon_32, %function
        .global yc_separate_tile_neon_32_y_only
        .type   yc_separate_tile_neon_32_y_only %function        

# note:
# (!): Use this to update the pointer after loading or storing, ready to load or store the next elements.
#      The increment is equal to the number of bytes read or written by the instruction.
#
# (, Rm): After the memory access, the pointer is incremented by the value in register Rm.
#  This is useful when reading or writing groups of elements that are separated by fixed widths,
#  eg. when reading a vertical line of data from an image.

yc_separate_tile_neon_256:
        push         {r0-r3, lr}

        # preload instruction
        #pld          [r2, #0x0]
        # load de-interleave
        vld2.8       {q0-q1}, [r2]!
        vld2.8       {q2-q3}, [r2]!
        vld2.8       {q4-q5}, [r2]!
        vld2.8       {q6-q7}, [r2]!
        vld2.8       {q8-q9}, [r2]!
        vld2.8       {q10-q11}, [r2]!
        vld2.8       {q12-q13}, [r2]!
        vld2.8       {q14-q15}, [r2]!
        
        # preload instruction
        #pldw         [r0, #0]
        vstm         r0, {q0}
        ADD 				 r0, r0, #512
        vstm         r0, {q2}
        ADD          r0, r0, #512
        vstm         r0, {q4}
        ADD          r0, r0, #512
        vstm         r0, {q6}
        ADD          r0, r0, #512
        vstm         r0, {q8}
        ADD          r0, r0, #512
        vstm         r0, {q10}
        ADD          r0, r0, #512
        vstm         r0, {q12}
        ADD          r0, r0, #512
        vstm         r0!, {q14}

        # preload instruction
        #pldw         [r1, #0]
        vstm         r1, {q1}
        ADD 				 r1, r1, #512
        vstm         r1, {q3}
        ADD 				 r1, r1, #512
        vstm         r1, {q5}
        ADD 				 r1, r1, #512
        vstm         r1, {q7}
        ADD 				 r1, r1, #512
        vstm         r1, {q9}
        ADD 				 r1, r1, #512
        vstm         r1, {q11}
        ADD 				 r1, r1, #512
        vstm         r1, {q13}
        ADD 				 r1, r1, #512
        vstm         r1, {q15}
        
        pop          {r0-r3, pc}

yc_separate_tile_neon_256_y_only:

        push         {r0-r3, lr}
        # preload instruction
        #pld          [r2, #0x0]
        # load de-interleave
        vld2.8       {q0-q1}, [r2]!
        vld2.8       {q2-q3}, [r2]!
        vld2.8       {q4-q5}, [r2]!
        vld2.8       {q6-q7}, [r2]!
        vld2.8       {q8-q9}, [r2]!
        vld2.8       {q10-q11}, [r2]!
        vld2.8       {q12-q13}, [r2]!
        vld2.8       {q14-q15}, [r2]!

        # preload instruction
        #pldw         [r0, #0]
        vstm         r0, {q0}
        ADD 				 r0, r0, #512
        vstm         r0, {q2}
        ADD 				 r0, r0, #512
        vstm         r0, {q4}
        ADD 				 r0, r0, #512
        vstm         r0, {q6}
        ADD 				 r0, r0, #512
        vstm         r0, {q8}
        ADD 				 r0, r0, #512
        vstm         r0, {q10}
        ADD 				 r0, r0, #512
        vstm         r0, {q12}
        ADD 				 r0, r0, #512
        vstm         r0, {q14}

        pop          {r0-r3, pc}

yc_separate_tile_neon_128:
        push         {r0-r3, lr}

        # preload instruction
        #pld          [r2, #0x0]
        # load de-interleave
        vld2.8       {q0-q1}, [r2]!
        vld2.8       {q2-q3}, [r2]!
        vld2.8       {q4-q5}, [r2]!
        vld2.8       {q6-q7}, [r2]!
        
        # preload instruction
        #pldw         [r0, #0]
        vstm         r0, {q0}
        ADD 				 r0, r0, #512
        vstm         r0, {q2}
        ADD          r0, r0, #512
        vstm         r0, {q4}
        ADD          r0, r0, #512
        vstm         r0, {q6}

        # preload instruction
        #pldw         [r1, #0]
        vstm         r1, {q1}
        ADD 				 r1, r1, #512
        vstm         r1, {q3}
        ADD 				 r1, r1, #512
        vstm         r1, {q5}
        ADD 				 r1, r1, #512
        vstm         r1, {q7}
        
        pop          {r0-r3, pc}

yc_separate_tile_neon_128_y_only:

        push         {r0-r3, lr}
        # preload instruction
        #pld          [r2, #0x0]
        # load de-interleave
        vld2.8       {q0-q1}, [r2]!
        vld2.8       {q2-q3}, [r2]!
        vld2.8       {q4-q5}, [r2]!
        vld2.8       {q6-q7}, [r2]!

        # preload instruction
        #pldw         [r0, #0]
        vstm         r0, {q0}
        ADD 				 r0, r0, #512
        vstm         r0, {q2}
        ADD 				 r0, r0, #512
        vstm         r0, {q4}
        ADD 				 r0, r0, #512
        vstm         r0, {q6}

        pop          {r0-r3, pc}
        
yc_separate_tile_neon_64:
        push         {r0-r3, lr}

        # preload instruction
        #pld          [r2, #0x0]
        # load de-interleave
        vld2.8       {q0-q1}, [r2]!
        vld2.8       {q2-q3}, [r2]!
        
        # preload instruction
        #pldw         [r0, #0]
        vstm         r0, {q0}
        ADD 				 r0, r0, #512
        vstm         r0, {q2}

        # preload instruction
        #pldw         [r1, #0]
        vstm         r1, {q1}
        ADD 				 r1, r1, #512
        vstm         r1, {q3}
        
        pop          {r0-r3, pc}

yc_separate_tile_neon_64_y_only:

        push         {r0-r3, lr}
        # preload instruction
        #pld          [r2, #0x0]
        # load de-interleave
        vld2.8       {q0-q1}, [r2]!
        vld2.8       {q2-q3}, [r2]!

        # preload instruction
        #pldw         [r0, #0]
        vstm         r0, {q0}
        ADD 				 r0, r0, #512
        vstm         r0, {q2}

        pop          {r0-r3, pc}

yc_separate_tile_neon_32:
        push         {r0-r3, lr}

        # preload instruction
        #pld          [r2, #0x0]
        # load de-interleave
        vld2.8       {q0-q1}, [r2]!
        
        # preload instruction
        #pldw         [r0, #0]
        vstm         r0, {q0}

        # preload instruction
        #pldw         [r1, #0]
        vstm         r1, {q1}
        
        pop          {r0-r3, pc}

yc_separate_tile_neon_32_y_only:

        push         {r0-r3, lr}
        # preload instruction
        #pld          [r2, #0x0]
        # load de-interleave
        vld2.8       {q0-q1}, [r2]!

        # preload instruction
        #pldw         [r0, #0]
        vstm         r0, {q0}

        pop          {r0-r3, pc}

