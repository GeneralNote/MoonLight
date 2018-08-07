#ifndef __MOONLIGHT_OBJ_MODEL_H__
#define __MOONLIGHT_OBJ_MODEL_H__

#include <MoonLight/Base/Bounds.h>
#include <MoonLight/Model/Model.h>
#include <unordered_map>
#include <vector>

namespace ml
{
	class OBJModel : public Model
	{
	public:
		OBJModel();
		~OBJModel();

		// .mtl files
		__declspec(align(16))	// align it so that it can be used as a data input for cbuffer
		struct Material
		{
			Material()
			{
				Ambient = DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f);
				Diffuse = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 0.0f);
				Specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
			}

			DirectX::XMFLOAT4 Diffuse;	// float4(RGB, Transparency/d)
			DirectX::XMFLOAT4 Specular;	// float4(RGB, Exponent)
			DirectX::XMFLOAT3 Ambient;	// float3(RGB)
		};
		struct MaterialData
		{
			MaterialData() {}

			MaterialData(const std::string& name) :
				Name(name)
			{}

			std::string Name;
			Material Material;
			std::string Texture;
		};

		// Vertex that can represent all the .obj files
		struct Vertex
		{
			DirectX::XMFLOAT3 Position;
			DirectX::XMFLOAT3 Normal;
			DirectX::XMFLOAT2 UV;
		};

		// load .obj file
		virtual bool LoadFromMemory(const char* data, ml::UInt32 dataLen);

		// get vertex list
		inline Vertex* GetVertexData() { return mVertices; }
		
		// get vertex data for a group/object
		Vertex* GetObjectVertices(const std::string& str);
		Vertex* GetGroupVertices(const std::string& str);

		// get vertex count
		inline size_t GetVertexCount() { return mVertCount; }

		// get vertex count for a group/object
		size_t GetObjectVertexCount(const std::string& str);
		size_t GetGroupVertexCount(const std::string& str);

		// get vertex start for a group/object
		size_t GetObjectVertexStart(const std::string& str);
		size_t GetGroupVertexStart(const std::string& str);

		// get object/group list
		inline std::vector<std::string> GetObjects() { return mObjects; }
		inline std::vector<std::string> GetGroups() { return mGroups; }

		// delete all the vertices from RAM
		void Release();

		// get material for a group/object
		std::pair<OBJModel::MaterialData*, ml::UInt32>* GetObjectMaterials(const std::string& obj);
		std::pair<OBJModel::MaterialData*, ml::UInt32>* GetGroupMaterials(const std::string& obj);
		ml::UInt32 GetObjectMaterialCount(const std::string& obj);
		ml::UInt32 GetGroupMaterialCount(const std::string& obj);

		// get material using its name
		MaterialData* GetMaterialByName(const std::string& name);

		// get material list
		inline std::vector<MaterialData> GetMaterialList() { return mMaterials; }

		// get object bounds
		inline ml::Bounds3D GetBounds() { return mBounds; }
		ml::Bounds3D GetObjectBounds(const std::string& obj);
		ml::Bounds3D GetGroupBounds(const std::string& grp);

	private:
		bool mLoadMaterialLibrary(const std::string& filename);

		Vertex* mVertices;
		ml::UInt32 mVertCount;
		std::vector<std::string> mObjects, mGroups;
		std::unordered_map<std::string, std::vector<std::pair<MaterialData*, ml::UInt32>>> mObjectMat, mGroupMat;
		std::vector<ml::UInt32> mObjectStartVerts, mGroupStartVerts;
		std::vector<MaterialData> mMaterials;

		bool mInitializedBounds;
		ml::Bounds3D mBounds;
	};
}

#endif //__MOONLIGHT_OBJ_MODEL_H__