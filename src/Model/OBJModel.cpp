#include <MoonLight/Model/OBJModel.h>
#include <algorithm>
#include <cctype>

// to shorten our mallocs:
using namespace DirectX;

#define MALLOC_STEP		100
#define CBUFFER_SIZE	100		// text buffer size
#define FBUFFER_SIZE	3		// float buffer size
#define IBUFFER_SIZE	9		// index/ml::UInt16 buffer size

namespace ml
{
	OBJModel::OBJModel()
	{
		mVertices = nullptr;
		mVertCount = 0;
		mInitializedBounds = false;
	}
	OBJModel::~OBJModel()
	{
		Release();
	}
	bool OBJModel::LoadFromMemory(const char * data, ml::UInt32 dataLen)
	{
		// free the memory first if neede
		Release();

		// get our copy
		char* code = const_cast<char*>(data);

		// buffer for saving temp data
		char cBuffer[CBUFFER_SIZE] = { 0 };
		char lastMaterial[CBUFFER_SIZE] = { 0 };
		float fBuffer[FBUFFER_SIZE] = { 0 };
		ml::SInt32 iBuffer[IBUFFER_SIZE] = { 0 };

		// count the number of 'F', 'V', 'VN', 'VT' commands that are in file
		// we do this so that we dont have to call realloc at all whis slowed
		// down this code
		ml::UInt32 vertAlloc = 0, vertPosAlloc = 0,
			vertNormAlloc = 0, vertUVAlloc = 0,
			objectAlloc = 0, groupAlloc = 0;

		while (code - data != dataLen) {
			if (*code == 'f' && isspace(*(code + 1)))
				vertAlloc++;
			else if (*code == 'v') {
				if (isspace(*(code + 1)))
					vertPosAlloc++;
				else if (*(code + 1) == 'n')
					vertNormAlloc++;
				else if (*(code + 1) == 't')
					vertUVAlloc++;
			}
			else if (*code == 'g')
				groupAlloc++;
			else if (*code == 'o')
				objectAlloc++;

			// skip the line
			while (*code != '\n')
				code++;
			code++;
		}
		code = const_cast<char*>(data);

		// vertex list
		mVertices = (Vertex*)malloc(vertAlloc * 3 * sizeof(Vertex));

		// vertex list
		ml::UInt16 vCount = 0;
		XMFLOAT3* v = (XMFLOAT3*)malloc(vertPosAlloc * sizeof(XMFLOAT3));

		// normal list
		ml::UInt16 vnCount = 0;
		XMFLOAT3* vn = (XMFLOAT3*)malloc(vertNormAlloc * sizeof(XMFLOAT3));

		// uv list
		ml::UInt16 vtCount = 0;
		XMFLOAT2* vt = (XMFLOAT2*)malloc(vertUVAlloc * sizeof(XMFLOAT2));

		// group & object list
		ml::UInt16 groups = 0, objects = 0;
		mGroups.resize(groupAlloc);
		mObjects.resize(objectAlloc);
		mGroupStartVerts.resize(groupAlloc);
		mObjectStartVerts.resize(objectAlloc);

		while (code-data != dataLen) {
			// check the first char in a line:

			// # -> comment
			if (*code == '#') {
				// do nothing
			}
			// v -> vertex position, vn -> vertex normal, vt -> vertex uv
			else if (*code == 'v') {
				code++;

				// v [float] [float] [float]
				if (isspace(*code)) {
					char *start = 0;

					// parse the 3 floats
					for (ml::UInt8 i = 0; i < 3; ++i) {
						start = code;
						
						// skip to next space
						do { code++; } while (!isspace(*code));

						memcpy(cBuffer, start, code - start);
						cBuffer[code - start] = 0;

						fBuffer[i] = atof(cBuffer);
					}
					v[vCount] = XMFLOAT3(fBuffer);

					// calculate the bounds
					if (mInitializedBounds) {
						mBounds.Min.x = std::min(mBounds.Min.x, fBuffer[0]);
						mBounds.Min.y = std::min(mBounds.Min.y, fBuffer[1]);
						mBounds.Min.z = std::min(mBounds.Min.z, fBuffer[2]);
						mBounds.Max.x = std::max(mBounds.Max.x, fBuffer[0]);
						mBounds.Max.y = std::max(mBounds.Max.y, fBuffer[1]);
						mBounds.Max.z = std::max(mBounds.Max.z, fBuffer[2]);
					} else {
						mInitializedBounds = true;
						mBounds.Min = mBounds.Max = v[vCount];
					}

					// increase the count
					vCount++;
				}
				// vn [float] [float] [float]
				else if (*code == 'n') {
					// skipe the letter 'n'
					code++;

					// parse the 3 floats
					char *start = 0;
					for (ml::UInt8 i = 0; i < 3; ++i) {
						start = code;

						// skip to next space
						do { code++; } while (!isspace(*code));

						memcpy(cBuffer, start, code - start);
						cBuffer[code - start] = 0;

						fBuffer[i] = atof(cBuffer);
					}
					vn[vnCount] = XMFLOAT3(fBuffer);

					// increase the count
					vnCount++;
				} 
				// vt [float] [float]
				else if (*code == 't') {
					// skip the letter 't'
					code++;

					// parse the 2 floats
					char *start = 0;
					for (ml::UInt8 i = 0; i < 2; ++i) {
						start = code;

						// skip to next space
						do { code++; } while (!isspace(*code));

						memcpy(cBuffer, start, code - start);
						cBuffer[code - start] = 0;

						fBuffer[i] = atof(cBuffer);
					}
					fBuffer[1] = 1 - fBuffer[1];
					vt[vtCount] = XMFLOAT2(fBuffer);

					// increase the count
					vtCount++;
				}
			}
			// f [vI]/[vtI]/[vnI] [vI]/[vtI]/[vnI] [vI]/[vtI]/[vnI]
			else if (*code == 'f') {
				code++;

				// reset the index buffer
				memset(iBuffer, -1, sizeof(SInt32) * IBUFFER_SIZE);

				// parse the 3 groups
				for (ml::UInt8 j = 0; j < 3; ++j) {
					// parse the v/vt/vn indices
					char *start = 0;
					for (ml::UInt8 i = 0; i < 3; ++i) {
						code++;
						start = code;

						// skip to next delimeter
						while ((i != 2 && *code != '/') || (i == 2 && !isspace(*code)))
							code++;

						memcpy(cBuffer, start, code - start);
						cBuffer[code - start] = 0;

						iBuffer[j*3+i] = atoi(cBuffer) - 1;
					}
				}

				// position is a must have
				mVertices[mVertCount].Position = v[iBuffer[0]];
				mVertices[mVertCount + 1].Position = v[iBuffer[3]];
				mVertices[mVertCount + 2].Position = v[iBuffer[6]];

				// default the UVs and normals
				mVertices[mVertCount].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
				mVertices[mVertCount + 1].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
				mVertices[mVertCount + 2].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

				mVertices[mVertCount].UV = DirectX::XMFLOAT2(0.0f, 0.0f);
				mVertices[mVertCount + 1].UV = DirectX::XMFLOAT2(0.0f, 0.0f);
				mVertices[mVertCount + 2].UV = DirectX::XMFLOAT2(0.0f, 0.0f);

				// set UVs if we have any
				if (iBuffer[1] != -1) {
					mVertices[mVertCount].UV = vt[iBuffer[1]];
					mVertices[mVertCount + 1].UV = vt[iBuffer[4]];
					mVertices[mVertCount + 2].UV = vt[iBuffer[7]];
				}

				// set normals if we have any
				if (iBuffer[2] != -1) {
					mVertices[mVertCount].Normal = vn[iBuffer[2]];
					mVertices[mVertCount + 1].Normal = vn[iBuffer[5]];
					mVertices[mVertCount + 2].Normal = vn[iBuffer[8]];
				}

				mVertCount += 3;
			}
			// g [group_name]
			else if (*code == 'g') {
				// skip to the text
				do { code++; } while (isspace(*code));

				// parse the 2 floats
				char *start = code;
				
				// skip to next space
				do { code++; } while (!isspace(*code));

				// copy the text
				memcpy(cBuffer, start, code - start);
				cBuffer[code - start] = 0;
				
				// save it
				mGroups[groups] = std::string(cBuffer);

				// save the vertex start
				mGroupStartVerts[groups] = mVertCount;

				// check if the previous object has a material and give it material if it doesnt have any
				if (groups > 0) {
					std::vector<std::pair<MaterialData*, UInt32>>& mats = mGroupMat[mGroups[groups - 1]];
					if (mats.empty())
						mats.push_back(std::make_pair(GetMaterialByName(std::string(lastMaterial)), mGroupStartVerts[groups-1]));
				}

				// increase the count
				groups++;
			}
			// o [object_name]
			else if (*code == 'o') {
				// skip to the text
				do { code++; } while (isspace(*code));

				// parse the 2 floats
				char *start = code;

				// skip to next space
				do { code++; } while (!isspace(*code));

				// copy the text
				memcpy(cBuffer, start, code - start);
				cBuffer[code - start] = 0;

				// save it
				mObjects[objects] = std::string(cBuffer);

				// save the vertex start
				mObjectStartVerts[objects] = mVertCount;

				// check if the previous object has a material and give it material if it doesnt have any
				if (objects > 0) {
					std::vector<std::pair<MaterialData*, UInt32>>& mats = mObjectMat[mObjects[objects - 1]];
					if (mats.empty())
						mats.push_back(std::make_pair(GetMaterialByName(std::string(lastMaterial)), mObjectStartVerts[objects - 1]));
				}

				// increase the count
				objects++;
			}
			// mtllib [filename]
			else if (*code == 'm') {
				code++;
				if (*code == 't') {
					do { code++; } while (!isspace(*code));
					code++;

					char* start = code;

					do { code++; } while (!isspace(*code));

					memcpy(cBuffer, start, code - start);
					cBuffer[code - start] = 0;

					mLoadMaterialLibrary(std::string(cBuffer));
				}
			}
			// usemtl [name]
			else if (*code == 'u') {
				code++;

				if (*code == 's' && mMaterials.size() != 0) {

					// first check if previous set of faces in this group/object have material
					if (objects > 0) {
						std::vector<std::pair<MaterialData*, UInt32>>& mats = mObjectMat[mObjects[objects - 1]];

						// check if the previous set of faces in this group dont have material
						if (mats.empty() && mVertCount - mObjectStartVerts[objects - 1] != 0)
							mats.push_back(std::make_pair(GetMaterialByName(std::string(lastMaterial)), mObjectStartVerts[objects - 1]));
					}
					if (groups > 0) {
						std::vector<std::pair<MaterialData*, UInt32>>& mats = mGroupMat[mGroups[groups - 1]];

						// check if the previous set of faces in this group dont have material
						if (mats.empty() && mVertCount - mGroupStartVerts[groups - 1] != 0)
							mats.push_back(std::make_pair(GetMaterialByName(std::string(lastMaterial)), mGroupStartVerts[groups - 1]));
					}

					do { code++; } while (!isspace(*code));
					code++;

					char* start = code;

					do { code++; } while (!isspace(*code));

					memcpy(lastMaterial, start, code - start);
					lastMaterial[code - start] = 0;

					if (objects > 0)
						mObjectMat[mObjects[objects - 1]].push_back(std::make_pair(GetMaterialByName(std::string(lastMaterial)), mVertCount));
					if (groups > 0)
						mGroupMat[mGroups[groups - 1]].push_back(std::make_pair(GetMaterialByName(std::string(lastMaterial)), mVertCount));
				}
			}

			// skip the line
			while (*code != '\n')
				code++;
			code++;
		}

		// free the memory
		free(v);
		free(vn);
		free(vt);

		return false;
	}
	OBJModel::Vertex * OBJModel::GetObjectVertices(const std::string & str)
	{
		for (size_t i = 0; i < mObjects.size(); i++)
			if (mObjects[i] == str)
				return mVertices + mObjectStartVerts[i];
		return nullptr;
	}
	OBJModel::Vertex * OBJModel::GetGroupVertices(const std::string & str)
	{
		for (size_t i = 0; i < mGroups.size(); i++)
			if (mGroups[i] == str)
				return mVertices + mGroupStartVerts[i];
		return nullptr;
	}
	size_t OBJModel::GetObjectVertexCount(const std::string & str)
	{
		for (size_t i = 0; i < mObjects.size(); i++)
			if (mObjects[i] == str) {
				if (i == mObjects.size() - 1)
					return mVertCount - mObjectStartVerts[i];
				else
					return mObjectStartVerts[i + 1] - mObjectStartVerts[i];
			}
		return 0;
	}
	size_t OBJModel::GetGroupVertexCount(const std::string & str)
	{
		for (size_t i = 0; i < mGroups.size(); i++)
			if (mGroups[i] == str) {
				if (i == mGroups.size() - 1)
					return mVertCount - mGroupStartVerts[i];
				else
					return mGroupStartVerts[i + 1] - mGroupStartVerts[i];
			}
		return 0;
	}
	size_t OBJModel::GetObjectVertexStart(const std::string & str)
	{
		for (size_t i = 0; i < mObjects.size(); i++)
			if (mObjects[i] == str)
				return mObjectStartVerts[i];
		return 0;
	}
	size_t OBJModel::GetGroupVertexStart(const std::string & str)
	{
		for (size_t i = 0; i < mGroups.size(); i++)
			if (mGroups[i] == str)
				return mGroupStartVerts[i];
		return 0;
	}
	void OBJModel::Release()
	{
		// free the memory if sth is already loaded
		if (mVertices != nullptr) {
			free(mVertices);
			mVertices = nullptr;
		}
		mVertCount = 0;

		mObjects.clear();
		mGroups.clear();
		mObjectStartVerts.clear();
		mGroupStartVerts.clear();

		mObjectMat.clear();
		mGroupMat.clear();
		mMaterials.clear();

		mBounds = ml::Bounds3D();
		mInitializedBounds = false;
	}
	std::pair<OBJModel::MaterialData*, ml::UInt32>* OBJModel::GetObjectMaterials(const std::string & obj)
	{
		if (mObjectMat.count(obj) > 0)
			return mObjectMat[obj].data();
		return nullptr;
	}
	std::pair<OBJModel::MaterialData*, ml::UInt32>* OBJModel::GetGroupMaterials(const std::string & obj)
	{
		if (mGroupMat.count(obj) > 0)
			return mGroupMat[obj].data();
		return nullptr;
	}
	ml::UInt32 OBJModel::GetObjectMaterialCount(const std::string & obj)
	{
		if (mObjectMat.count(obj) > 0)
			return mObjectMat[obj].size();
		return 0;
	}
	ml::UInt32 OBJModel::GetGroupMaterialCount(const std::string & obj)
	{
		if (mGroupMat.count(obj) > 0)
			return mGroupMat[obj].size();
		return 0;
	}
	OBJModel::MaterialData* OBJModel::GetMaterialByName(const std::string & name)
	{
		for (int i = 0; i < mMaterials.size(); i++)
			if (mMaterials[i].Name == name)
				return &mMaterials[i];
		return nullptr;
	}
	ml::Bounds3D OBJModel::GetObjectBounds(const std::string & obj)
	{
		Vertex* verts = GetObjectVertices(obj);
		ml::UInt32 cnt = GetObjectVertexCount(obj);

		if (cnt != 0 && verts != nullptr) {
			ml::Bounds3D ret(verts[0].Position, verts[0].Position);
			for (ml::UInt32 i = 1; i < cnt; i++) {
				ret.Min.x = std::min(ret.Min.x, verts[i].Position.x);
				ret.Min.y = std::min(ret.Min.y, verts[i].Position.y);
				ret.Min.z = std::min(ret.Min.z, verts[i].Position.z);
				ret.Max.x = std::max(ret.Max.x, verts[i].Position.x);
				ret.Max.y = std::max(ret.Max.y, verts[i].Position.y);
				ret.Max.z = std::max(ret.Max.z, verts[i].Position.z);
			}

			return ret;
		}

		return ml::Bounds3D();
	}
	ml::Bounds3D OBJModel::GetGroupBounds(const std::string & grp)
	{
		Vertex* verts = GetGroupVertices(grp);
		ml::UInt32 cnt = GetGroupVertexCount(grp);

		if (cnt != 0 && verts != nullptr) {
			ml::Bounds3D ret(verts[0].Position, verts[0].Position);
			for (ml::UInt32 i = 1; i < cnt; i++) {
				ret.Min.x = std::min(ret.Min.x, verts[i].Position.x);
				ret.Min.y = std::min(ret.Min.y, verts[i].Position.y);
				ret.Min.z = std::min(ret.Min.z, verts[i].Position.z);
				ret.Max.x = std::max(ret.Max.x, verts[i].Position.x);
				ret.Max.y = std::max(ret.Max.y, verts[i].Position.y);
				ret.Max.z = std::max(ret.Max.z, verts[i].Position.z);
			}

			return ret;
		}

		return ml::Bounds3D();
	}
	bool OBJModel::mLoadMaterialLibrary(const std::string & filename)
	{
		// open file
		FILE *file = fopen(filename.c_str(), "rb");

		if (file == nullptr)
			return false;

		// get file size
		fseek(file, 0, SEEK_END);
		long bytecodeLen = ftell(file);
		fseek(file, 0, SEEK_SET);

		// read bytecode
		char *bytecode = (char*)malloc(bytecodeLen);
		fread(bytecode, bytecodeLen, 1, file);

		// close file
		fclose(file);

		// make a pointer copy
		char* data = bytecode;

		// buffer
		char cBuffer[CBUFFER_SIZE] = { 0 };
		float fBuffer[4] = { 0.0f };

		// parse
		while (data - bytecode != bytecodeLen) {
			// check the first char in a line:

			// # -> comment
			if (*data == '#') {
				// do nothing
			}
			// newmtl [name]
			else if (*data == 'n') {
				data++;
				if (*data == 'e') {

					// skip the unneeded info
					while (*data != ' ' && *data != '\t')
						data++;
					data++;

					char* start = data;

					while (!isspace(*data))
						data++;

					memcpy(cBuffer, start, data - start);
					cBuffer[data - start] = 0;

					mMaterials.push_back(MaterialData(std::string(cBuffer)));
				}
			}
			// K[letter]
			else if (*data == 'K') {
				data++;

				// Kd [float] [float] [float] -> diffuse
				if (*data == 'd') {
					// skip the letter 'd'
					data++;

					// parse the 3 floats
					char *start = 0;
					for (ml::UInt8 i = 0; i < 3; ++i) {
						start = data;

						// skip to next space
						do { data++; } while (!isspace(*data));

						memcpy(cBuffer, start, data - start);
						cBuffer[data - start] = 0;

						fBuffer[i] = atof(cBuffer);
					}
					fBuffer[3] = 1.0f;
					mMaterials[mMaterials.size()-1].Material.Diffuse = XMFLOAT4(fBuffer);
				}
				// Ka [float] [float] [float] -> ambient
				else if (*data == 'a') {
					// skip the letter 'a'
					data++;

					// parse the 3 floats
					char *start = 0;
					for (ml::UInt8 i = 0; i < 3; ++i) {
						start = data;

						// skip to next space
						do { data++; } while (!isspace(*data));

						memcpy(cBuffer, start, data - start);
						cBuffer[data - start] = 0;

						fBuffer[i] = atof(cBuffer);
					}
					mMaterials[mMaterials.size() - 1].Material.Ambient = XMFLOAT3(fBuffer);
				}
				// Ks [float] [float] [float] -> specular
				else if (*data == 's') {
					// skip the letter 's'
					data++;

					// parse the 3 floats
					char *start = 0;
					for (ml::UInt8 i = 0; i < 3; ++i) {
						start = data;

						// skip to next space
						do { data++; } while (!isspace(*data));

						memcpy(cBuffer, start, data - start);
						cBuffer[data - start] = 0;

						fBuffer[i] = atof(cBuffer);
					}
					fBuffer[3] = 0;
					mMaterials[mMaterials.size() - 1].Material.Specular = XMFLOAT4(fBuffer);
				}
			}
			// d [float] -> transparency
			else if (*data == 'd') {
				// skip the letter 'd'
				data++;

				// parse the float
				char *start = data;

				// skip to next space
				do { data++; } while (!isspace(*data));
				
				memcpy(cBuffer, start, data - start);
				cBuffer[data - start] = 0;

				mMaterials[mMaterials.size() - 1].Material.Diffuse.w = atof(cBuffer);
			}
			// Tr [float] -> 1-transparency
			else if (*data == 'T') {
				// skip the letter 'T' and 'r'
				data += 2;

				// parse the float
				char *start = data;

				// skip to next space
				do { data++; } while (!isspace(*data));

				memcpy(cBuffer, start, data - start);
				cBuffer[data - start] = 0;

				mMaterials[mMaterials.size() - 1].Material.Diffuse.w = 1 - atof(cBuffer);
			}
			// N
			else if (*data == 'N') {
				// skip the letter 'N'
				data++;

				// Ns [float] -> specular power
				if (*data == 's') {
					// parse the float
					char *start = data;

					// skip to next space
					do { data++; } while (!isspace(*data));

					memcpy(cBuffer, start, data - start);
					cBuffer[data - start] = 0;

					mMaterials[mMaterials.size() - 1].Material.Specular.w = atof(cBuffer);
				}
			}
			// map_*
			if (*data == 'm') {
				// skip the letter 'm'
				data++;

				// parse the float
				char *start = data;

				// skip to next space
				do { data++; } while (!isspace(*data));

				memcpy(cBuffer, start, data - start);
				cBuffer[data - start] = 0;

				// map_Kd - diffuse map
				if (strcmp(cBuffer, "ap_Kd") == 0) {
					// skip the letter 'm'
					data++;

					// parse the float
					start = data;

					// skip to next space
					do { data++; } while (!isspace(*data));

					if (data - start > 1) {
						memcpy(cBuffer, start, data - start);
						cBuffer[data - start] = 0;

						mMaterials[mMaterials.size() - 1].Texture = std::string(cBuffer);
					}
				}
			}

			// skip the line
			while (*data != '\n')
				data++;
			data++;
		}

		// free the memory
		free(bytecode);

		return true;
	}
}