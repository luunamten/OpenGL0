#ifndef D_MODEL_TOOL
#define D_MODEL_TOOL

#include <string>
#include <vector>
#include <assimp\Importer.hpp>

class ModelTool {
public:
	enum class Material {
		ambient, diffuse, specular
	};	

	ModelTool(std::string path);
	~ModelTool() = default;
	void LoadModelC(
		std::vector<float> &coordVector);
	void LoadModelN(
		std::vector<float> &coordVector);
	void LoadModelT(
		std::vector<float> &coordVector);
	void LoadModelCN(
		std::vector<float> &coordVector,
		std::vector<float> &normalVector);
	void LoadModelCNT(
		std::vector<float> &coordVector,
		std::vector<float> &normalVector,
		std::vector<float> &texVector);
	void LoadModelCNT2D(
		std::vector<float> &coordVector,
		std::vector<float> &normalVector,
		std::vector<float> &texVector);
	void LoadModelM3V(
		std::vector<float>& propertyValueVector,
		const Material& mType);
	void LoadModelM4V(
		std::vector<float>& propertyValueVector,
		const Material& mType);

private:
	void P_LoadModelM3V(
		std::vector<float>& propertyValueVector,
		const char* pKey, int a1, int a2);
	void P_LoadModelM4V(
		std::vector<float>& propertyValueVector,
		const char* pKey, int a1, int a2);
	void LoadModelMaterial(
		std::vector<float>& propertyValueVector, const Material& mType,
		void(ModelTool::* executor)
		(
			std::vector<float>&,
			const char* pKey, int type, int idx
		)
	);

	Assimp::Importer importer;
};
#endif