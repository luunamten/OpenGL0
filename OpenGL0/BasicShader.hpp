#pragma once
class BasicShader
{
private:
	unsigned int m_Program;
	int mu_TransformMat;
	int mu_RotationMat;
	int mu_AmbientFactor;
	int mu_LightDir;
	int mu_LightColor;
	int mu_DefaultColor;
public:
	BasicShader(const char* vtPath, const char* frPath);
	~BasicShader();
	void InitShader(const char* vtPath, const char* frPath);
	void UseProgram() const;
	void UnUseProgram() const;
	unsigned int Program() const;
	int u_TransformMat() const;
	int u_RotationMat() const;
	int u_AmbientFactor() const;
	int u_LightDir() const;
	int u_LightColor() const;
	int u_DefaultColor() const;
};

