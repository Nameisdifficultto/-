// class12.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <vector>
using std::vector;

//���Ѱ· ��bfs��   (dfs)

//1����ֱ�ߣ�������б��
//2�������·����ȱ�Ȼ�ҵ��������·��
//3�����Ĳ�δ������
//		ÿ������ָ���Ľڵ㣬�������ܱ�����δ���ʵĽڵ�����������У���ͨ�������������������һ���δ���ʽڵ㣬������µ��������У��Դ����ƣ�ֱ��Ѱ·��ֹ


#define MAP_ROW 8
#define MAP_COL 10

enum PathDir{p_up,p_down,p_left,p_right};

struct MyPoint
{
	int row, col;
};

struct PathData
{
	PathDir dir;
	int val;
	bool isFind;
};

struct MyPathNode//���νṹ�Ľڵ�����
{
	MyPoint pos;//�����򣬼�¼�����п�ͨ�е�·�������Ϣ
	MyPathNode *parent;//���ڵ�ָ�룬��ǰ����ڵ�ĸ��ڵ���˭
	vector<MyPathNode *> childNode;//����һ������������������ʾһ����̬���飬���������ŵ���MyPathNode *ָ��
};

bool isMove(PathData pArr[][MAP_COL],MyPoint const& pos)//�ж�pos��������Ƿ���pArr�����ǿ���ͨ�е�
{
	//�ж������Ƿ�Խ��
	if (pos.row >= 0 && pos.col >= 0 && pos.row < MAP_ROW && pos.col < MAP_COL)
	{
		if (pArr[pos.row][pos.col].val == 0//�ж϶�Ӧ��λ�ò������ϰ�
			&& pArr[pos.row][pos.col].isFind == false)//�ж϶�Ӧ��λ����û�б�����
			return true;
	}
	return false;
}

void clearTree(MyPathNode *&root)
{
	if (root)
	{
		//ɾ���Լ�֮ǰҪ��ɾ���Լ�����һ��ڵ�
		for (size_t i = 0; i < root->childNode.size(); ++i)
			clearTree(root->childNode[i]);
		delete root;
		root = nullptr;//root����Ϊ���ã��������︳�գ���ı�ʵ��
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int arr[MAP_ROW][MAP_COL] = {
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	};

	PathData pathArr[MAP_ROW][MAP_COL] = {};
	for (int i = 0; i < MAP_ROW; ++i)
	{
		for (int j = 0; j < MAP_COL; ++j)
		{
			pathArr[i][j].isFind = false;
			pathArr[i][j].dir = p_up;
			pathArr[i][j].val = arr[i][j];
		}
	}

	MyPoint beginPoint = { 1, 1 };
	MyPoint endPoint = {6,8};

	pathArr[beginPoint.row][beginPoint.col].isFind = true;//�����㱻����

	//׼��һ��������������Ѱ·�е���������
	MyPathNode *pRoot = new MyPathNode;//׼��һ�����ĸ��ڵ�
	pRoot->pos = beginPoint;//���ĸ��ڵ���������������
	pRoot->parent = nullptr;//���ڵ�û�и��ڵ�
	//���ڵ���ӽڵ���˭�����ﲻ������������ڵ��ٽ�����Щ��ͨ�е�·��

	//��1���ص�  ׼�������������飬�����������Ǹ��������������νṹ�е�������׵�ַ
	vector<MyPathNode *> nodeList;//���ڲ��ҵĵ�ǰ�㣬���Ϊ��ɫָ����ָ�Ĳ�
	vector<MyPathNode *> tempNodeList;//��ǰ�ڵ���Ҫ��������һ��Ľڵ㣬���Ϊ��ɫָ����ָ�Ĳ�

	nodeList.push_back(pRoot);//�����ڲ��ҵĵ�ǰ�㣬��Ӹ��ڵ���׵�ַ
	//nodeList[0]->pos;
	MyPoint tempPoint;//��ʱ���꣬������¼һ�µ�ǰ�����һ��λ�ã������жϵ�ǰ�����һ��λ���Ƿ����ͨ��

	while (true)//�����Ҫѭ�����ٴ�
	{
		//��2���ص㣺ѭ����������,ͨ��������������������Ԫ�ظ������ж�ѭ�����ٴ�
		for (int i = 0; i < (int)nodeList.size(); ++i)
		{
			for (int j = 0; j < 4; ++j)//4����
			{
				tempPoint = nodeList[i]->pos;//�Ѵ����������������Ԫ����ָ����������ʱ���걣��
				switch (j)
				{
				case p_up:
					tempPoint.row--;
					break;
				case p_down:
					tempPoint.row++;
					break;
				case p_left:
					tempPoint.col--;
					break;
				case p_right:
					tempPoint.col++;
					break;
				}
				//�õ��˵�ǰ�������һ��4�������ʱ����
				if (isMove(pathArr, tempPoint))
				{
					//�������㣬֤�����λ���ǿ���ͨ��
					//��3���ص�   ׼���������ι�ϵ
					//��׼��һ���ڵ�
					MyPathNode *pNode = new MyPathNode;
					pNode->pos = tempPoint;//�������ͨ�е��������һ����ʱ�Ľڵ���

					//ע�⣺ �������ӹ�ϵ
					pNode->parent = nodeList[i];//�ӽڵ�������ڵ�
					nodeList[i]->childNode.push_back(pNode);//���ڵ��ϵ�ӽڵ�

					pathArr[tempPoint.row][tempPoint.col].isFind = true;//�����ͨ�нڵ㣬�Ѿ����������νṹ����Ϊ�ѷ���
					//��4���ص�  ����������˵�ǰ������λ�õĽڵ��׵�ַ��ӽ���һ�δ�����������
					tempNodeList.push_back(pNode);

					//��Ϊ��״�ṹ���������±��������㣬�������Ϸ��㣬ֻ��һ�����ڵ�
					if (tempPoint.row == endPoint.row && tempPoint.col == endPoint.col)
					{
						//��ʾ�ҵ��յ�
						MyPathNode *tempNode = pNode;
						while (pNode)
						{
							printf("row = %d, col = %d\n",pNode->pos.row,pNode->pos.col);
							pNode = pNode->parent;
						}
						goto LABLE;
					}
				}
			}
		}

		if (tempNodeList.size() == 0)//û��·�ˣ�û�����յ㣬��һ�εĴ�����������Ϊ��
			break;
		//��5���ص�
		nodeList = tempNodeList; //��һ�δ����������鸳ֵ����ǰ�����������飬����ɫ��ָ���鸳ֵ����ɫ����
		tempNodeList.clear();//�����һ�δ�����������
	}

LABLE:
	clearTree(pRoot);
	return 0;
}

