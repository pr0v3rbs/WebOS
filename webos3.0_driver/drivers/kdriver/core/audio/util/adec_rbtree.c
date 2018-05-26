/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
*/

#include "adec_rbtree.h"
#include "common/adec_common.h"

/******************************************************************************
  구조체
 ******************************************************************************/

AdecMemStat *IMC_MEM_MOD;

typedef struct _RBNode
{
	RBKey key;					// 키
	void* info;					// 내용
	RB_COLOR color;				// 노드 색깔 : RED or BLACK
	struct _RBNode* parent;		// 부모 노드
	struct _RBNode* left;		// 왼쪽 자식 노드
	struct _RBNode* right;		// 오른쪽 자식 노드
}RBNode;

typedef struct _RBTree
{
	RBNode* root;
	RBNode* sentinal;
	void	(*terminator)(void*);
}RBTree;

/******************************************************************************
  지역 함수 선언
 ******************************************************************************/
// 트리 내 노드 제거
static ADEC_RESULT _RBTree_DeleteAllRBNode(
	RBTree* _tree,
	RBNode* _node);
// 왼쪽 회전
static ADEC_INLINE ADEC_RESULT _RBTree_LeftRotate(
	RBTree* _tree,
	RBNode* _node);

// 오른쪽 회전
static ADEC_INLINE ADEC_RESULT _RBTree_RightRotate(
	RBTree* _tree,
	RBNode* _node);
// Terminator
static ADEC_INLINE void _list_default_terminator(void *_item);

#if 0
// 순회
static void _RBTree_Traverse(
	RBNode* _node,
	VisitFuncPtr_Key _key,
	VisitFuncPtr_Info _info);
#endif

/******************************************************************************
  지역 함수
 ******************************************************************************/
// 트리 내 노드 제거
static ADEC_RESULT _RBTree_DeleteAllRBNode(RBTree* _tree, RBNode* _node)
{
	void	(*trm)(void*);

	trm = _tree->terminator;

	if(_node != _tree->sentinal && _node != NULL)
	{
		_RBTree_DeleteAllRBNode(_tree, _node->left);
		_RBTree_DeleteAllRBNode(_tree, _node->right);
		trm(_node->info);
		ADEC_FREE(IMC_MEM_MOD, _node);
	}

	return ADEC_ERR_NONE;
}

// 왼쪽 회전
static ADEC_INLINE ADEC_RESULT _RBTree_LeftRotate(
	RBTree* _tree,
	RBNode* _node)
{
	RBNode* parentNode = _node->parent;
	RBNode* childNode = _node->left;
	RBNode* sentinal = _tree->sentinal;

	if(parentNode == parentNode->parent->left)	// 할배(부모)쪽		노드 등록
	{
		parentNode->parent->left = _node;
	}
	else if(parentNode == parentNode->parent->right)
	{
		parentNode->parent->right = _node;
	}

	_node->parent = parentNode->parent;			// 노드쪽			할배(부모) 등록
	_node->left = parentNode;					// 노드쪽			부모(자식) 등록
	parentNode->parent = parentNode->right;		// 부모(자식)쪽		노드(부모) 등록
	parentNode->right = childNode;				// 부모쪽			자식 등록
	if(childNode != sentinal)
	{
		childNode->parent = parentNode;			// 자식쪽			부모 등록 (센티널이 아닌경우)
	}

	// 루트 노드 교체의 경우
	if(parentNode == _tree->root)
	{
		_node->color = RB_COLOR_BLACK;
		_tree->root = _node;
	}

	return ADEC_ERR_NONE;
}
// 오른쪽 회전
static ADEC_INLINE ADEC_RESULT _RBTree_RightRotate(
	RBTree* _tree,
	RBNode* _node)
{
	RBNode* parentNode = _node->parent;
	RBNode* childNode = _node->right;
	RBNode* sentinal = _tree->sentinal;

	if(parentNode == parentNode->parent->left)	// 할배(부모)쪽		노드 등록
	{
		parentNode->parent->left = _node;
	}
	else if(parentNode == parentNode->parent->right)
	{
		parentNode->parent->right = _node;
	}

	_node->parent = parentNode->parent;			// 노드쪽			할배(부모) 등록
	_node->right = parentNode;					// 노드쪽			부모(자식) 등록
	parentNode->parent = parentNode->left;		// 부모(자식)쪽		노드(부모) 등록
	parentNode->left = childNode;				// 부모쪽			자식 등록
	if(childNode != sentinal)
	{
		childNode->parent = parentNode;			// 자식쪽			부모 등록 (센티널이 아닌경우)
	}

	// 루트 노드 교체의 경우
	if(parentNode == _tree->root)
	{
		_node->color = RB_COLOR_BLACK;
		_tree->root = _node;
	}

	return ADEC_ERR_NONE;
}

static ADEC_INLINE void _list_default_terminator(void *_item)
{
	return;
}
#if 0
// 순회
static void _RBTree_Traverse(
	RBNode* _node,
	VisitFuncPtr_Key _key,
	VisitFuncPtr_Info _info)
{
	if(_node ==  NULL)
	{
		return;
	}

	_key(_node->key);
	_info(_node->info);
	_RBTree_Traverse(_node->left, _key, _info);
	_RBTree_Traverse(_node->right, _key, _info);
}
#endif

/******************************************************************************
  전역함수
 ******************************************************************************/
// 트리 생성
ADEC_RESULT RBTree_CreateTree(void** _tree, void (*_terminator)(void*))
{
	RBTree* tree;
	int structSize = sizeof(RBTree);

	// memory allocation
	*_tree = tree = ADEC_MALLOC(IMC_MEM_MOD, structSize, RBTree);

	return RBTree_InitTree(tree, _terminator);
}

// 트리 초기화
ADEC_RESULT	RBTree_InitTree(void* _tree, void (*_terminator)(void*))
{
	RBTree* tree;

	tree = (RBTree*)_tree;

#if 0
	tree->root = ADEC_MALLOC(IMC_MEM_MOD, sizeof(RBNode), RBNode);
#endif
	tree->sentinal = ADEC_MALLOC(IMC_MEM_MOD, sizeof(RBNode), RBNode);

	if (_terminator == NULL)
	    tree->terminator = _list_default_terminator;
	else
	    tree->terminator = _terminator;

	// 센티널 노드 초기화
	tree->sentinal->key = 0;
	tree->sentinal->color = RB_COLOR_BLACK;
	tree->sentinal->parent = tree->sentinal;
	tree->sentinal->left = tree->sentinal;
	tree->sentinal->right = tree->sentinal;

	tree->root = NULL;
#if 0
	// 루트 노트 초기화
	tree->root->key = 0;
	tree->root->color = RB_COLOR_BLACK;
	tree->root->parent = tree->sentinal;
	tree->root->left = tree->sentinal;
	tree->root->right = tree->sentinal;
#endif

	ADEC_ASSERT( _tree!= NULL, return ADEC_ERR_NULL);

	return ADEC_ERR_NONE;
}

// 트리 제거
ADEC_RESULT RBTree_DeleteTree(void* _tree)
{
	RBTree* tree;

	tree = (RBTree*)_tree;

	ADEC_ASSERT(
		_tree != NULL,
		return ADEC_ERR_NULL);

	_RBTree_DeleteAllRBNode(tree, tree->root);
	ADEC_FREE(IMC_MEM_MOD,tree->sentinal);
	ADEC_FREE(IMC_MEM_MOD,tree);

	return ADEC_ERR_NONE;
}

// 노드 생성
ADEC_RESULT RBTree_CreateNode(void* _tree, RBKey _key, void* _info)
{
	RBTree* tree;
	RBNode* parentNode;			// 부모 노드
	RBNode* uncleNode = NULL;	// 삼촌 노드
	RBNode* currentNode;		// 현재 노드
	RBNode* newNode;			// 생성 노드

	tree = (RBTree*)_tree;

	ADEC_ASSERT(
		_tree != NULL && _info != NULL,
		return ADEC_ERR_NULL);

	// 잘못된 값 삽입
	if (_key == 0)
	{
	    return ADEC_ERR_RANGE;
	}

	parentNode = tree->sentinal;
	currentNode = tree->root;

	// 새로운 노드가 추가될 위치 찾기
	while(currentNode != tree->sentinal && tree->root != NULL)
	{
		// 중복키 체크
		if(_key == currentNode->key)
		{
			currentNode->info = _info;
			return ADEC_ERR_DUPLICATED;
		}

		parentNode = currentNode;

		// 키 크기비교를 통한 주소 찾기
		currentNode = (currentNode->key > _key) ? currentNode->left : currentNode->right;
	}

	// 새로운 자식 노드 생성
	newNode = ADEC_MALLOC(IMC_MEM_MOD, sizeof(RBNode), RBNode);
	newNode->key =  _key;
	newNode->left = tree->sentinal;
	newNode->right = tree->sentinal;
	newNode->info = _info;

	if(parentNode != tree->sentinal)	// 루트 노드가 아닌경우
	{
#if 0
		newNode = ADEC_MALLOC(IMC_MEM_MOD, sizeof(RBNode), RBNode);
		newNode->key =  _key;
		newNode->left = tree->sentinal;
		newNode->right = tree->sentinal;
		newNode->info = _info;
#endif
		newNode->color = RB_COLOR_RED;
		newNode->parent = parentNode;

		// 생성된 노드와 부모-자식 관계 등록
		if(_key < newNode->parent->key)
		{
			newNode->parent->left = newNode;
		}
		else
		{
			newNode->parent->right = newNode;
		}

		currentNode = newNode;

		while(currentNode->parent->color == RB_COLOR_RED)					// 부모가 RED일 경우
		{
			// 부모가 할배의 왼쪽 자식일 경우
			if(currentNode->parent == currentNode->parent->parent->left)
			{
				uncleNode = currentNode->parent->parent->right;				// 삼촌은 할배의 오른쪽 자식
				// case 1: 삼촌이 RED 일 경우
				if(uncleNode->color == RB_COLOR_RED)
				{
					if(currentNode->parent->parent != tree->root)
					{
						currentNode->parent->parent->color = RB_COLOR_RED;	// 할배 반전
					}
					currentNode->parent->color = RB_COLOR_BLACK;			// 부모 반전
					uncleNode->color = RB_COLOR_BLACK;						// 삼촌 반전

					currentNode = currentNode->parent->parent;				// 재귀
				}
				// case 2 : 삼촌이 BLACK 일 경우
				else
				{
					if(currentNode == currentNode->parent->right)			// 현재 노드가 안쪽일 경우
					{
						_RBTree_LeftRotate(tree, currentNode);				// 바깥쪽으로 작은 회전
						currentNode = currentNode->left;
					}
					currentNode->parent->color = RB_COLOR_BLACK;			// 부모 반전
					currentNode->parent->parent->color = RB_COLOR_RED;		// 할배 반전
					_RBTree_RightRotate(tree, currentNode->parent);			// 큰 회전
				}
			}
			// 부모가 할배의 오른쪽 자식일 경우
			else
			{
				uncleNode = currentNode->parent->parent->left;				// 삼촌은 할배의 왼쪽 자식
				// case 1 : 삼촌이 RED 일 경우
				if(uncleNode->color == RB_COLOR_RED)
				{
					if(currentNode->parent->parent != tree->root)
					{
						currentNode->parent->parent->color = RB_COLOR_RED;	// 할배 반전
					}
					currentNode->parent->color = RB_COLOR_BLACK;			// 부모 반전
					uncleNode->color = RB_COLOR_BLACK;						// 삼촌 반전

					currentNode = currentNode->parent->parent;				// 재귀
				}
				// case 2 : 삼촌이 BLACK 일 경우
				else
				{
					if(currentNode == currentNode->parent->left)			// 현재 노드가 안쪽일 경우
					{
						_RBTree_RightRotate(tree, currentNode);				// 바깥쪽으로 작은 회전
						currentNode = currentNode->right;
					}
					currentNode->parent->color = RB_COLOR_BLACK;			// 부모 반전
					currentNode->parent->parent->color = RB_COLOR_RED;		// 할배 반전
					_RBTree_LeftRotate(tree, currentNode->parent);			// 큰 회전
				}
			}
		}
	}
	else	// 새 노드가 루트일 경우
	{
		newNode->color = RB_COLOR_BLACK;
		newNode->parent = parentNode;

		tree->root =  newNode;
#if 0
		tree->root->key =  _key;
		tree->root->color = RB_COLOR_BLACK;
		tree->root->info = _info;
#endif
	}

	return ADEC_ERR_NONE;
}

// 키 탐색
ADEC_RESULT RBTree_Search(void* _tree, RBKey _key, void** _info)
{
	RBTree* tree;
	RBNode* currentNode;

	ADEC_ASSERT(_info != NULL, return ADEC_ERR_NULL);

	tree = (RBTree*)_tree;
	currentNode = tree->root;

	while(currentNode != tree->sentinal && tree->root != NULL)
	{
		// 찾았으면 해당 노드 리턴
		if(_key == currentNode->key)
		{
			*_info = currentNode->info;
			return ADEC_ERR_NONE;
		}
		// 못찾았으면 키 크기 비교 후 이동
		else
		{
			currentNode = (_key < currentNode->key) ? currentNode->left : currentNode->right;
		}
	}

	// 트리에 찾는 키가 없는 경우
	*_info = NULL;

	return ADEC_ERR_NOT_FOUND;
}

// 키 삭제
ADEC_RESULT RBTree_Delete(void* _tree, RBKey _key)
{
	RBTree* tree;
	RBNode* currentNode;
	RBNode* deleteNode;
	RBNode* childNode;	// 삭제 대상의 자식 노드
	RBNode* replaceNode;
	void	(*trm)(void*);

	// Check a NULL pointer.
	if((void*)_tree == NULL)
	{
		return ADEC_ERR_NOT_FOUND;
	}

	tree = (RBTree*)_tree;
	trm = tree->terminator;

	// Check a NULL pointer.
	if(tree->root == NULL)
	{
		return ADEC_ERR_NOT_FOUND;
	}

	currentNode = tree->root;

	// 삭제 대상 노드 탐색
	while(currentNode != tree->sentinal && currentNode->key != _key) // 센티널까지 가거나, 키를 찾을때까지
	{
		currentNode = (_key < currentNode->key) ? currentNode->left : currentNode->right;
	}

	// 탐색 실패시
	if(currentNode == tree->sentinal)
	{
		return ADEC_ERR_NOT_FOUND;
	}

	// 탐색 성공시
	deleteNode = currentNode;
	trm(currentNode->info);

	// case 1 : 삭제 대상이 단말 노드
	if(deleteNode->left == tree->sentinal && deleteNode->right == tree->sentinal)
	{

		if(deleteNode == tree->root)	// 루트 노드일 경우
		{
//			tree->root->key = 0;	// 키 초기화
			ADEC_FREE(IMC_MEM_MOD,tree->root);
			tree->root = NULL;
			return ADEC_ERR_NONE;
		}
		else
		{
			// 노드 삭제 : 센티
			if(deleteNode->parent->left == deleteNode)
			{
				deleteNode->parent->left = tree->sentinal;
			}
			else
			{
				deleteNode->parent->right = tree->sentinal;
			}
		}

		replaceNode = deleteNode;
	}
	// case 2 : 삭제 대상이 하나의 자식 노드를 가질 경우
	else if(deleteNode->left == tree->sentinal || deleteNode->right == tree->sentinal)
	{
		// 삭제 대상의 자식 노드 파악
		childNode = (deleteNode->left != tree->sentinal) ? deleteNode->left : deleteNode->right;

		// 삭제 대상 위치에 삭제 대상 자식으로 교체
		if (deleteNode == tree->root)					// 삭제 대상이 루트
		{
			childNode->color = RB_COLOR_BLACK;
			childNode->parent = tree->root->parent;
			tree->root = childNode;						// 새로운 루트 등록
		}
		else
		{
			if(deleteNode->parent->left == deleteNode)	// 삭제 대상이 부모의 왼쪽 자식
			{
				deleteNode->parent->left = childNode;
				childNode->parent = deleteNode->parent;
			}
			else											// 삭제 대상이 부모의 오른쪽 자식
			{
				deleteNode->parent->right = childNode;
				childNode->parent = deleteNode->parent;
			}
		}

		replaceNode = deleteNode;
	}
	// case 3 : 삭제 대상이 두개의 자식 노드를 가질 경우 : 오른쪽 서브 트리의 최소값으로 교체 후 교체 노드 삭제.
	else
	{
		// 대체 노드 값을 삭제 노드 값에 대입
		replaceNode = deleteNode->right;	// 대체 노드

		// 대체 노드 탐색 : 오른쪽 서브 트리의 최소값
		while(replaceNode->left != tree->sentinal)
		{
			replaceNode = replaceNode->left;
		}

		currentNode = replaceNode->right;						// 대체 노드의 자식

		// 대체 노드의 부모 노드와 자식 노드 연결
		if(replaceNode->parent->left == replaceNode)			// 대체 노드가 대체 노드 부모의 왼쪽 자식
		{
			replaceNode->parent->left = currentNode;
			if(currentNode != tree->sentinal)
			{
				currentNode->parent= replaceNode->parent;
			}
		}
		else													// 대체 노드가 대체 노드 부모의 오른쪽 자식
		{
			replaceNode->parent->right = currentNode;
			if(currentNode != tree->sentinal)
			{
				currentNode->parent = replaceNode->parent;
			}
		}

		// 삭제 노드에 대체 노드 값 대입
		deleteNode->key = replaceNode->key;
		deleteNode->info = replaceNode->info;

		// 밸런스 및 색 조정
		if(replaceNode->color == RB_COLOR_BLACK)
		{
			while(currentNode->color == RB_COLOR_BLACK && currentNode != tree->sentinal && currentNode != tree->root)
			{
				// case 1 : 부모가 적색, 조카둘이 흑색(루트 까지 반복)
				if(currentNode->parent->color == RB_COLOR_RED
					&& currentNode->parent->left->left->color == RB_COLOR_BLACK
					&& currentNode->parent->left->right->color == RB_COLOR_BLACK)
				{
					currentNode->parent->color = RB_COLOR_BLACK;
					currentNode->parent->left->color = RB_COLOR_RED;

					currentNode = currentNode->parent;
				}
				else
				{
					// case 1: 형제가 적색
					if(currentNode->parent->left->color == RB_COLOR_RED)
					{
						_RBTree_RightRotate(tree, currentNode->parent->left);
						currentNode->parent->color = RB_COLOR_RED;
						currentNode->parent->parent->color = RB_COLOR_BLACK;
					}

					// case 2 : 부모가 흑색, 조카둘이 흑색
					else if(currentNode->parent->color == RB_COLOR_BLACK
						&& currentNode->parent->left->left->color == RB_COLOR_BLACK
						&& currentNode->parent->left->right->color == RB_COLOR_BLACK)
					{
						currentNode->parent->left->color = RB_COLOR_RED;
					}

					// case 3 : 형제의 안쪽 자식이 적색, 바깥쪽 자식이 흑색
					else if(currentNode->parent->left->left->color == RB_COLOR_BLACK
						&& currentNode->parent->left->right->color == RB_COLOR_RED)
					{
						_RBTree_LeftRotate(tree, currentNode->parent->left->right);

						currentNode->parent->left->color = RB_COLOR_BLACK;
						currentNode->parent->left->left->color = RB_COLOR_RED;
					}
					// case 4 : 형제의 바깥쪽 자식이 적색
					else if(currentNode->parent->left->left->color == RB_COLOR_RED)
					{
						currentNode->parent->left->left->color = RB_COLOR_BLACK;

						_RBTree_RightRotate(tree, currentNode->parent->left);
					}
					currentNode = tree->root;
				}
			}
			currentNode->color = RB_COLOR_BLACK;
		}
	}

	ADEC_FREE(IMC_MEM_MOD,replaceNode);

	return ADEC_ERR_NONE;
}
