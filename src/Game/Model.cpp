#include "stdafx.h"
#include "Model.h"

glm::vec2 ConvertToGLM(const aiVector2D& vec)
{
	return glm::vec2(vec.x, vec.y);
}

glm::vec3 ConvertToGLM(const aiVector3D& vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec3 ConvertToGLM(const aiColor3D& vec)
{
	return glm::vec3(vec.r, vec.g, vec.b);
}

glm::quat ConvertToGLM(const aiQuaternion& vec)
{
	return glm::quat(vec.w, vec.x, vec.y, vec.z);
}

glm::mat4 ConvertToGLM(const aiMatrix4x4& mat)
{
	return glm::mat4(
		mat.a1, mat.b1, mat.c1, mat.d1,
		mat.a2, mat.b2, mat.c2, mat.d2,
		mat.a3, mat.b3, mat.c3, mat.d3,
		mat.a4, mat.b4, mat.c4, mat.d4
	);
}


void CopyNode(
	const aiScene* assimpscene, aiNode* assimpnode,
	const std::vector<Material>& materials, const std::vector<DrawElementsBaseVertex>& meshes, Node& node)
{
	node.Name = string(assimpnode->mName.C_Str());
	node.Transform = ConvertToGLM(assimpnode->mTransformation);

	for (unsigned int i = 0; i < assimpnode->mNumMeshes; i++)
	{
		unsigned int meshindex = assimpnode->mMeshes[i];
		if (meshindex >= meshes.size())
		{
			cout << "ERROR: meshindex out of range: " << meshindex << endl;
			continue;
		}
		unsigned int materialindex = assimpscene->mMeshes[meshindex]->mMaterialIndex;
		if (materialindex >= materials.size())
		{
			cout << "ERROR: materialindex out of range: " << materialindex << endl;
			continue;
		}

		node.Drawables.push_back({ materials[materialindex], meshes[meshindex] });
	}

	/* children */
	for (unsigned int i = 0; i < assimpnode->mNumChildren; i++)
	{
		Node* child = node.AddChild();
		CopyNode(assimpscene, assimpnode->mChildren[i], materials, meshes, *child);
	}
}


bool Model::Load(const std::string& filename)
{
	/* reset model */


	/* load model */
	unsigned int flags =
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType;

	Assimp::Importer assimpimporter;
	const aiScene* assimpscene = assimpimporter.ReadFile(filename, flags);
	if (!assimpscene || assimpscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		/* failure */
		return false;


	/* get base path of model (for textures) */
	size_t found = filename.find_last_of("/\\");
	string base_path = found != string::npos ? filename.substr(0, found) + "/" : "";


	/* extract data ... */
	/************************************************************************/
	vector<Material> materials;
	vector<DrawElementsBaseVertex> meshes;

	/* materials */
	for (unsigned int i = 0; i < assimpscene->mNumMaterials; i++)
	{
		aiMaterial* material = assimpscene->mMaterials[i];
		materials.push_back({});

		aiColor3D diffuse, specular;
		float shininess = 0, shininessstrength = 0;

		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
			materials.back().Kd = ConvertToGLM(diffuse);
		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, specular))
			materials.back().Ks = ConvertToGLM(specular);

		if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess))
			materials.back().Ns = shininess;
		if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessstrength))
			materials.back().Ks *= shininessstrength;

		/* texture */
		/* by default, set texture to NULL_TEXTURE so that:
			Kd * texture = Kd in case no texture is used for this material */
		materials.back().map_Kd = Graphics::LoadTexture(NULL_TEXTURE_NAME);

		for (unsigned int j = 0; j < material->GetTextureCount(aiTextureType_DIFFUSE); j++)
		{
			aiString texture;
			if (AI_SUCCESS == material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, j), texture))
			{
				materials.back().map_Kd = Graphics::LoadTexture(base_path + texture.C_Str());
				/* is texture is used, reset Kd to vec3(1, 1, 1) so that:
					Kd * texture = texture
				(the reason of this is to avoid materials with and without textures) */
				if (materials.back().map_Kd != Graphics::LoadTexture(NULL_TEXTURE_NAME))
					materials.back().Kd = vec3(1, 1, 1);
			}
		}
	}

	/* meshes */
	for (unsigned int i = 0; i < assimpscene->mNumMeshes; i++)
	{
		aiMesh* mesh = assimpscene->mMeshes[i];

		Mesh<Vertex::PTN> meshdata;
		meshdata.Name = mesh->mName.C_Str();

		/* vertices */
		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			/* vertex positions */
			vec3 position = ConvertToGLM(mesh->mVertices[j]);
			vec2 texcoord = vec2(0, 0);
			vec3 normal = vec3(0, 0, 0);

			/* vertex normals */
			if (mesh->HasNormals())
				normal = ConvertToGLM(mesh->mNormals[j]);

			meshdata.Vertices.push_back({ position, texcoord, normal });
		}

		/* faces */
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			if (face.mNumIndices == 3)
			{
				meshdata.Indices.push_back(face.mIndices[0]);
				meshdata.Indices.push_back(face.mIndices[1]);
				meshdata.Indices.push_back(face.mIndices[2]);
			}
		}

		/* UV channels */
		for (unsigned int j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; j++)
		{
			if (!mesh->HasTextureCoords(j))
				continue;

			aiVector3D* texcoords = mesh->mTextureCoords[j];
			if (mesh->mNumUVComponents[j] == 2)
			{
				for (unsigned int k = 0; k < mesh->mNumVertices; k++)
					/* flip texcoord.y for GL texture coordinate system */
					meshdata.Vertices[k].TexCoord = vec2(texcoords[k].x, 1 - texcoords[k].y);
			}
		}

		meshes.push_back(Graphics::AddMesh(meshdata));
	}

	/* scene nodes */
	CopyNode(assimpscene, assimpscene->mRootNode, materials, meshes, Root);

	/* animations */
	for (unsigned int i = 0; i < assimpscene->mNumAnimations; i++)
	{
		aiAnimation* animation = assimpscene->mAnimations[i];
		Animations.push_back({});
		Animations.back().Name = animation->mName.C_Str();
		Animations.back().Duration = animation->mDuration;
		Animations.back().TicksPerSecond = animation->mTicksPerSecond;

		/* channels */
		for (unsigned int j = 0; j < animation->mNumChannels; j++)
		{
			aiNodeAnim* channel = animation->mChannels[j];
			Node* nodefound = Root.Find(string(channel->mNodeName.C_Str()));
			if (!nodefound)
			{
				cout << "ERROR: cant find node for animation: " << channel->mNodeName.C_Str() << endl;
				continue;
			}

			Animations.back().Channels.push_back({});
			Animations.back().Channels.back().NodeID = nodefound->ID();


			map<double, Transformation> keymap;

			/* position keys */
			for (unsigned int k = 0; k < channel->mNumPositionKeys; k++)
			{
				aiVectorKey key = channel->mPositionKeys[k];
				keymap[key.mTime].Position = ConvertToGLM(key.mValue);
			}

			/* rotation keys */
			for (unsigned int k = 0; k < channel->mNumRotationKeys; k++)
			{
				aiQuatKey key = channel->mRotationKeys[k];
				keymap[key.mTime].Rotation = ConvertToGLM(key.mValue);
			}

			for (auto& key : keymap)
			{
				/* calculate animation relative to base transform */
				/* (A * B)^-1 = B^-1 * A^-1 */
				mat4 A = inverse(nodefound->Transform.Matrix()) * key.second.Matrix();

				Animations.back().Channels.back().Keys.push_back({ key.first, Transformation(A) });
			}

			/* pre-/post state */
			if (channel->mPreState == aiAnimBehaviour_CONSTANT)
				Animations.back().Channels.back().PreState = Animation::Channel::State::Constant;
			else if (channel->mPreState == aiAnimBehaviour_LINEAR)
				Animations.back().Channels.back().PreState = Animation::Channel::State::Linear;
			else if (channel->mPreState == aiAnimBehaviour_REPEAT)
				Animations.back().Channels.back().PreState = Animation::Channel::State::Repeat;
			else
				Animations.back().Channels.back().PreState = Animation::Channel::State::Default;

			if (channel->mPreState == aiAnimBehaviour_CONSTANT)
				Animations.back().Channels.back().PostState = Animation::Channel::State::Constant;
			else if (channel->mPreState == aiAnimBehaviour_LINEAR)
				Animations.back().Channels.back().PostState = Animation::Channel::State::Linear;
			else if (channel->mPreState == aiAnimBehaviour_REPEAT)
				Animations.back().Channels.back().PostState = Animation::Channel::State::Repeat;
			else
				Animations.back().Channels.back().PostState = Animation::Channel::State::Default;
		}
	}
	/************************************************************************/

	/* success */
	return true;
}


void Model::Animate(double animation_time, unsigned int animation_index)
{
	/* check if animation exists */
	if (animation_index >= Animations.size())
		return;

	/* check if animation is valid */
	if (Animations[animation_index].Duration <= 0.01)
		return;

	/* restart animation when finished */
	animation_time = fmod(animation_time, Animations[animation_index].Duration);

	/* calculate animated model nodes */
	auto animated_nodes = Animations[animation_index].Animate(animation_time);
	for (auto& animated_node : animated_nodes)
	{
		auto node = Root.Find(animated_node.NodeID);
		if (!node)
		{
			cout << "ERROR: cant find animated node " << animated_node.NodeID << endl;
			continue;
		}

		/* set animated transformation */
		node->Animation = animated_node.Transform;
	}
}


void Model::Draw(std::vector<DrawableNode>& drawable_nodes) const
{
	Root.Draw(drawable_nodes);
}
