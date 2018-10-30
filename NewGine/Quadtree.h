#pragma once
#include "MathGeoLib\include\MathGeoLib.h"

class GameObject;
class QuadNode
{
public:
	QuadNode(float3 min, float3 max);
	QuadNode(QuadNode* parent);

	~QuadNode();

	void Clean();
	void DebugDraw();

	bool InsertNode(GameObject* go);
	bool RemoveNode(GameObject* go);
	void CreateChildNodes();


	template <typename T>
	std::vector<GameObject*> GetColliderObjects(T type);

	AABB GetBox();
	void SetBox(int index, float3 limit);

private:
	QuadNode * parent;
	std::vector<QuadNode> childs;
	std::vector<GameObject*> gos;
	AABB aabb;

};


class Quadtree
{
public:
	Quadtree(float3 min, float3 max);
	~Quadtree();

	void DebugDraw();


	void Insert(GameObject* go);
	void Remove(GameObject* go);

	template <typename t>
	std::vector<GameObject*> GetColliderObjects(t type);

private:
	QuadNode root;
};

//QuadNode
template<typename T>
inline std::vector<GameObject*> QuadNode::GetColliderObjects(T type)
{
	std::vector<GameObject*> ret;
	if (aabb.Intersects(type))
	{
		if (gos.empty() == false)
		{
			for (std::vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); it++)
			{
				if ((*it)->IsActive() && (*it)->aabb.IsFinite() && type.Intersects((*it)->aabb) == true)
				{
					ret.push_back(*it);
				}
			}
		}
		if (childs.empty() == false)
		{
			for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
			{
				std::vector<GameObject*> toAdd = it->GetColliderObjects(type);
				if (toAdd.empty() == false)
				{
					for (std::vector<GameObject*>::iterator it = toAdd.begin(); it != toAdd.end(); it++)
					{
						ret.push_back(*it);
					}
				}
			}
		}
	}
	return ret;
}


//QuadTree
template<typename t>
inline std::vector<GameObject*> Quadtree::GetColliderObjects(t type)
{
	return root.GetColliderObjects(type);
}