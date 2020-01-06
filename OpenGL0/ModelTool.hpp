#ifndef D_MODEL_TOOL
#define D_MODEL_TOOL

#include <string>
#include <vector>
#include <assimp\Importer.hpp>

class ModelTool {
private:
	Assimp::Importer importer;
public:	
	ModelTool(std::string path);
	~ModelTool() = default;
	void loadModelC(
		std::vector<float> &coordVector);
	void loadModelN(
		std::vector<float> &coordVector);
	void loadModelT(
		std::vector<float> &coordVector);
	void loadModelCN(
		std::vector<float> &coordVector,
		std::vector<float> &normalVector);
	void loadModelCNT(
		std::vector<float> &coordVector,
		std::vector<float> &normalVector,
		std::vector<float> &texVector);
	void loadModelCNT2D(
		std::vector<float> &coordVector,
		std::vector<float> &normalVector,
		std::vector<float> &texVector);
	void loadModelM3V(
		std::vector<float>& propertyValueVector,
		const char * pKey, int a1, int a2);
	void loadModelM4V(
		std::vector<float>& propertyValueVector,
		const char * pKey, int a1, int a2);
};
#endif