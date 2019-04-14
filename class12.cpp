// class12.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
using std::vector;

//广度寻路 （bfs）   (dfs)

//1、走直线，不能走斜线
//2、如果有路，广度必然找到的是最短路径
//3、树的层次次序遍历
//		每次搜索指定的节点，并将其周边所有未访问的节点加入搜索队列，再通这个搜索队列再搜索下一层的未访问节点，来组成新的搜索队列，以此类推，直到寻路终止


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

struct MyPathNode//树形结构的节点类型
{
	MyPoint pos;//数据域，记录着所有可通行的路径点的信息
	MyPathNode *parent;//父节点指针，当前这个节点的父节点是谁
	vector<MyPathNode *> childNode;//定义一个容器对象，这个对象表示一个动态数组，数组里面存放的是MyPathNode *指针
};

bool isMove(PathData pArr[][MAP_COL],MyPoint const& pos)//判断pos这个坐标是否在pArr里面是可以通行的
{
	//判断坐标是否越界
	if (pos.row >= 0 && pos.col >= 0 && pos.row < MAP_ROW && pos.col < MAP_COL)
	{
		if (pArr[pos.row][pos.col].val == 0//判断对应的位置不能是障碍
			&& pArr[pos.row][pos.col].isFind == false)//判断对应的位置有没有被访问
			return true;
	}
	return false;
}

void clearTree(MyPathNode *&root)
{
	if (root)
	{
		//删除自己之前要先删除自己的下一层节点
		for (size_t i = 0; i < root->childNode.size(); ++i)
			clearTree(root->childNode[i]);
		delete root;
		root = nullptr;//root参数为引用，所以这里赋空，会改变实参
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

	pathArr[beginPoint.row][beginPoint.col].isFind = true;//标记起点被访问

	//准备一棵树，用来保存寻路中的所有数据
	MyPathNode *pRoot = new MyPathNode;//准备一棵树的根节点
	pRoot->pos = beginPoint;//树的根节点的数据是起点数据
	pRoot->parent = nullptr;//根节点没有父节点
	//根节点的子节点是谁？这里不做，不清楚根节点临近有哪些可通行的路径

	//第1个重点  准备两个辅助数组，这两个仅仅是辅助用来保存树形结构中的两层的首地址
	vector<MyPathNode *> nodeList;//正在查找的当前层，理解为蓝色指针所指的层
	vector<MyPathNode *> tempNodeList;//当前节点需要搜索的下一层的节点，理解为紫色指针所指的层

	nodeList.push_back(pRoot);//在正在查找的当前层，添加根节点的首地址
	//nodeList[0]->pos;
	MyPoint tempPoint;//临时坐标，用来记录一下当前点的下一个位置，便于判断当前点的下一个位置是否可以通行

	while (true)//不清楚要循环多少次
	{
		//第2个重点：循环查找数组,通过这个待搜索数组里面的元素个数来判断循环多少次
		for (int i = 0; i < (int)nodeList.size(); ++i)
		{
			for (int j = 0; j < 4; ++j)//4方向
			{
				tempPoint = nodeList[i]->pos;//把待搜索的数组里面的元素所指的坐标用临时坐标保存
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
				//得到了当前坐标的下一个4方向的临时坐标
				if (isMove(pathArr, tempPoint))
				{
					//条件满足，证明这个位置是可以通行
					//第3个重点   准备构建树形关系
					//先准备一个节点
					MyPathNode *pNode = new MyPathNode;
					pNode->pos = tempPoint;//把这个可通行的坐标放在一个临时的节点中

					//注意： 关联父子关系
					pNode->parent = nodeList[i];//子节点关联父节点
					nodeList[i]->childNode.push_back(pNode);//父节点关系子节点

					pathArr[tempPoint.row][tempPoint.col].isFind = true;//这个可通行节点，已经关联进树形结构，改为已访问
					//第4个重点  把这个保存了当前可能行位置的节点首地址添加进下一次待搜索的数组
					tempNodeList.push_back(pNode);

					//因为树状结构，从上往下遍历不方便，从下往上方便，只有一个根节点
					if (tempPoint.row == endPoint.row && tempPoint.col == endPoint.col)
					{
						//表示找到终点
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

		if (tempNodeList.size() == 0)//没有路了，没有找终点，下一次的待搜索的数组为空
			break;
		//第5个重点
		nodeList = tempNodeList; //下一次待搜索的数组赋值给当前待搜索的数组，把紫色所指数组赋值给蓝色数组
		tempNodeList.clear();//清空下一次待搜索的数组
	}

LABLE:
	clearTree(pRoot);
	return 0;
}

