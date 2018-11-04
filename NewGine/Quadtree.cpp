#include "Quadtree.h"
#include "GameObject.h"

QuadNode::QuadNode(float3 min, float3 max) : parent(nullptr)
{
	aabb.minPoint = min;
	aabb.maxPoint = max;
}

QuadNode::QuadNode(QuadNode* parent)
{
	aabb = parent->GetBox();
}


QuadNode::~QuadNode() 
{
	
}

void QuadNode::Clean()
{
	bool childsHaveChilds = false;
	std::vector<GameObject*> childsGOs;
	for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		if (it->childs.empty() == false)
		{
			//If a child has childs, we shouldn't erase any of them! Just in case
			childsHaveChilds = true;
			break;
		}
		for (std::vector<GameObject*>::iterator childIt = it->gos.begin(); childIt != it->gos.end(); childIt++)
		{
			childsGOs.push_back(*childIt);
		}
	}

	if (childsHaveChilds == false)
	{
		if (childsGOs.empty() == true)
		{
			childs.clear();
		}
		else if (childsGOs.size() + gos.size() <= 1)
		{
			for (std::vector<GameObject*>::iterator it = childsGOs.begin(); it != childsGOs.end(); it++)
			{
				gos.push_back(*it);
			}
			childs.clear();
		}

		if (parent != nullptr)
		{
			parent->Clean();
		}
	}
}

void QuadNode::DebugDraw()
{
	float3 corners[8];
	aabb.GetCornerPoints(corners);
	//2DO draw box
	for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		it->DebugDraw();
	}
}

bool QuadNode::InsertNode(GameObject* go)
{
	bool ret = false;
	if (aabb.Intersects(go->aabb))
	{
		if (childs.empty() == true)
		{
			gos.push_back(go);

			if (gos.size() > 1)
			{
				CreateChildNodes();
			}
		}
		else
		{
			std::vector<QuadNode*> intersected_nodes;
			for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
			{
				if (it->aabb.Intersects(go->aabb))
				{
					intersected_nodes.push_back(&*it);
				}
			}
			if (intersected_nodes.size() == 1)
			{
				intersected_nodes.front()->InsertNode(go);
			}
			else if (intersected_nodes.size() > 1)
			{
				gos.push_back(go);
			}
		}
		ret = true;
	}
	return ret;
}

bool QuadNode::RemoveNode(GameObject* go)
{
	bool ret = false;
	if (gos.empty() == false)
	{
		for (std::vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); it++)
		{
			if ((*it) == go)
			{
				gos.erase(it);
				Clean();
				return true;
			}
		}
	}

	if (childs.empty() == false)
	{
		for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			ret = it->RemoveNode(go);
			if (ret == true)
			{
				break;
			}
		}
	}
	return ret;
}

void QuadNode::CreateChildNodes()
{
	float3 center = aabb.CenterPoint();

	for (int n = 0; n < 4; n++)
	{
		childs.push_back(QuadNode(this));
		childs.back().SetBox(n, center);
	}

	std::vector<GameObject*> tmp = gos;
	gos.clear();

	for (std::vector<GameObject*>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		InsertNode(*it);
	}
}


AABB QuadNode::GetBox()
{ 
	return aabb; 
}

void QuadNode::SetBox(int index, float3 limit)
{
	AABB parent_aabb = parent->GetBox();
	switch (index)
	{
		case 0:
		{
			aabb.minPoint.x = parent_aabb.minPoint.x;
			aabb.minPoint.z = limit.z;
			aabb.maxPoint.x = limit.x;
			aabb.maxPoint.z = parent_aabb.maxPoint.z;
			break;
		}
		case 1:
		{
			aabb.minPoint.x = limit.x;
			aabb.minPoint.z = limit.z;
			aabb.maxPoint.x = parent_aabb.maxPoint.x;
			aabb.maxPoint.z = parent_aabb.maxPoint.z;
			break;
		}
		case 2:
		{
			aabb.minPoint.x = limit.x;
			aabb.minPoint.z = parent_aabb.minPoint.z;
			aabb.maxPoint.x = parent_aabb.maxPoint.x;
			aabb.maxPoint.z = limit.z;
			break;
		}
		case 3:
		{
			aabb.minPoint.x = parent_aabb.minPoint.x;
			aabb.minPoint.z = parent_aabb.minPoint.z;
			aabb.maxPoint.x = limit.x;
			aabb.maxPoint.z = limit.z;
			break;
		}
	}
}


std::vector<QuadNode> QuadNode::GetChilds()
{
	return childs;
}



//===================================================================================

Quadtree::Quadtree(float3 min, float3 max) : root(min, max)
{

}

Quadtree::~Quadtree() 
{

}

void Quadtree::DebugDraw()
{
	root.DebugDraw();
}

QuadNode Quadtree::GetRoot()
{
	return root;
}



void Quadtree::Insert(GameObject* go)
{
	if (go->aabb.IsFinite())
	{
		root.InsertNode(go);
	}
}

void Quadtree::Remove(GameObject* go)
{
	if (go->aabb.IsFinite())
	{
		root.RemoveNode(go);
	}
}