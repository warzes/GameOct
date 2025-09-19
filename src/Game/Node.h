#pragma once

#include "Drawable.h"

//=============================================================================
// Node.h
//=============================================================================
class Node
{
public:

	/* data */
	std::string Name;
	Transformation Transform, Animation;
	std::vector<Drawable> Drawables;


	Node();
	virtual ~Node();

	glm::mat4 WorldTransform() const;

	/* identifier */
	unsigned int ID() const;
	unsigned int NumberOfNodes();

	/* get */
	Node* Find(unsigned int id);
	Node* Find(const std::string& name);
	Node* Root();
	Node* Parent();
	std::vector<Node*> Siblings();
	std::vector<Node*> Children();
	unsigned int Generation();
	void Draw(std::vector<DrawableNode>& drawable_nodes) const;

	/* check */
	bool IsAncestorOf(Node* node);
	bool IsDescendantOf(Node* node);

	/* modify */
	Node* AddChild();
	Node* AddChild(Node* nodesource);
	Node* AddChild(const Transformation& transform, const std::vector<Drawable>& drawables);

	/* print hierarchy */
	void Print(const std::string& left_distance = "");

private:

	Node(Node* parent, unsigned int id);

	/* not allowed to be copied or assigned to */
	Node(const Node& other) = delete;
	Node& operator=(const Node& other) = delete;

	/* data */
	Node* m_parent = nullptr;
	std::vector<Node*> m_children;
	unsigned int m_id = 0;

	/* recursion */
	unsigned int NumberOfSubNodes();

};