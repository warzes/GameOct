#include "stdafx.h"
#include "Node.h"

//=============================================================================
// Node.h
//=============================================================================
Node::Node(Node* parent, unsigned int id)
	: m_parent(parent), m_id(id)
{
	Name = "node " + std::to_string(m_id);
}


Node::Node()
	: m_id(1)
{
	Name = "node " + std::to_string(m_id);
}


Node::~Node()
{
	/* remove children */
	for (auto& child : m_children)
		delete child;

	m_children.clear();
}


glm::mat4 Node::WorldTransform() const
{
	return
		m_parent ?
		m_parent->WorldTransform() * Transform.Matrix() * Animation.Matrix() :
		Transform.Matrix() * Animation.Matrix();
}


unsigned int Node::ID() const
{
	return m_id;
}


unsigned int Node::NumberOfNodes()
{
	return Root()->NumberOfSubNodes();
}


Node* Node::Find(unsigned int id)
{
	if (m_id == id)
		return this;

	for (auto& child : m_children)
	{
		auto nodefound = child->Find(id);
		if (nodefound)
			return nodefound;
	}

	return nullptr;
}


Node* Node::Find(const std::string& name)
{
	if (Name == name)
		return this;

	for (auto& child : m_children)
	{
		auto nodefound = child->Find(name);
		if (nodefound)
			return nodefound;
	}

	return nullptr;
}


Node* Node::Root()
{
	Node* node = this;

	while (node->Parent())
		node = node->Parent();

	return node;
}


Node* Node::Parent()
{
	return m_parent;
}


std::vector<Node*> Node::Siblings()
{
	if (!m_parent)
		return {};

	return m_parent->m_children;
}


std::vector<Node*> Node::Children()
{
	return m_children;
}


unsigned int Node::Generation()
{
	unsigned int generation = 1;
	Node* node = this;

	while (node->Parent())
	{
		node = node->Parent();
		generation++;
	}

	return generation;
}


void Node::Draw(std::vector<DrawableNode>& drawable_nodes) const
{
	/* for each "Drawable" = material + mesh(drawcall) */
	for (auto& drawable : Drawables)
		/* tie it to the node transformation: make a "DrawableNode" */
		drawable_nodes.push_back(
			DrawableNode(WorldTransform(), drawable.material, drawable.mesh_drawcall));

	/* draw all children recursively */
	for (auto& child : m_children)
		child->Draw(drawable_nodes);
}


bool Node::IsAncestorOf(Node* node)
{
	return node ? node->IsDescendantOf(this) : false;
}


bool Node::IsDescendantOf(Node* node)
{
	if (node == nullptr || node == this)
		return false;

	if (node == m_parent)
		return true;

	return m_parent ? m_parent->IsDescendantOf(node) : false;
}


Node* Node::AddChild()
{
	return AddChild({}, {});
}


Node* Node::AddChild(Node* nodesource)
{
	if (!nodesource)
		return nullptr;

	/* if source node is part of this tree, ... */
	if (nodesource == this || nodesource->IsAncestorOf(this) || nodesource->IsDescendantOf(this))
	{
		/* copy node tree temporarily */
		Node node;
		auto child = node.AddChild(nodesource);
		return AddChild(child);
	}

	/* otherwise */
	auto nodechild = AddChild(nodesource->Transform, nodesource->Drawables);
	for (auto& nodesourcechild : nodesource->m_children)
		nodechild->AddChild(nodesourcechild);

	return nodechild;
}


Node* Node::AddChild(const Transformation& transform, const std::vector<Drawable>& drawables)
{
	m_children.push_back(new Node(this, NumberOfNodes() + 1));
	m_children.back()->Transform = transform;
	m_children.back()->Drawables = drawables;
	return m_children.back();
}


void Node::Print(const std::string& left_distance)
{
	//std::cout << left_distance << Name << " (ID:" << ID() << ")" << std::endl;

	for (auto& child : m_children)
		child->Print(left_distance + "\t");
}


unsigned int Node::NumberOfSubNodes()
{
	unsigned int nodecount = 1;

	for (auto& child : m_children)
		nodecount += child->NumberOfSubNodes();

	return nodecount;
}