#include <Windows.h>
#include<iostream>
#include "application/application.h"
#include "gpu/gpu.h"
#include "gpu/dataStructures.h"
#include "gpu/shader/lightShader.h"
#include "application/image.h"
#include "application/camera.h"
#include "application/model.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup" )

/*
* 1 Mesh
* 2 Model
* 3 example
*/
uint32_t WIDTH = 1920;
uint32_t HEIGHT = 1080;

Camera* camera = nullptr;

//ʹ�õ�Shader
LightShader* shader = nullptr;
DirectionalLight directionalLight;
math::vec3f envLight;

//ʹ�õ�ģ��
Model* model = nullptr;

float angle = 0;
void transform() {
	angle -= 0.02f;
	auto rotateMatrix = math::rotate(math::mat4f(), angle, {0.0f,1.0f,0.0f});
	auto translateMatrix = math::translate(math::mat4f(), 0.0f, 0.0f, -5.0f);
	auto m = translateMatrix * rotateMatrix;

	m = math::scale(m, 0.5f, 0.5f, 0.5f);
	m = math::translate(m, 0.0f, -2.8f, 0.45f);
	model->setModelMatrix(m);
}

void render() {
	transform();

	shader->mViewMatrix = camera->getViewMatrix();
	shader->mProjectionMatrix = camera->getProjectionMatrix();

	shader->mDirectionalLight = directionalLight;
	shader->mEnvLight = envLight;

	sgl->clear();
	sgl->useProgram(shader);
	model->draw(shader);
}

void prepare() {
	camera = new Camera(60.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f, { 0.0f, 1.0f, 0.0f });
	APP->setCamera(camera);

	shader = new LightShader();
	directionalLight.color = {1.0f, 1.0f, 1.0f};
	directionalLight.direction = {-1.0f, -0.5f, -0.7f};
	envLight = { 0.5f, 0.5f, 0.5f };

	sgl->enable(CULL_FACE);

	model = new Model();
	model->read("assets/model/dinosaur/source/Rampaging T-Rex.glb");
	//model->read("assets/model/Fist Fight B.fbx");
	//model->read("assets/model/bag/backpack.obj");

}

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,		//��Ӧ�ó���ʵ�������Ψһָ����ǰ����
	_In_opt_ HINSTANCE hPrevInstance,	//������ǰһ��ʵ����һ����null
	_In_ LPWSTR    lpCmdLine,		//Ӧ�ó������в���
	_In_ int       nCmdShow)		//���������ʾ����󻯡���С�������أ�����������
{
	if (!APP->initApplication(hInstance, WIDTH, HEIGHT)) {
		return -1;
	}

	//��bmpָ����ڴ����õ�sgl���� 
	sgl->initSurface(APP->getWidth(), APP->getHeight(), APP->getCanvas());

	prepare();

	bool alive = true;
	while (alive) {
		alive = APP->peekMessage();
		camera->update();

		render();
		APP->show();
	}

	delete shader;
	delete camera;

	return 0;
}
