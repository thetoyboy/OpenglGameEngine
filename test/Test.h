#ifndef _RGP_TEST_SAMPLE_H_
#define _RGP_TEST_SAMPLE_H_
#include "../Core/Graphics/Window.h"
#include "../Core/Graphics/GLRenderer.h"
#include "../Core/Graphics/Model3D.h"
#include "../Core/Scene/GameScene.h"
#include "../Core/Scene/DirectionnalLight.h"
#include "../Core/Scene/PointLight.h"
#include "../Core/Scene/SpotLight.h"


namespace RGP_CORE {
    class Test {
    public :
        Test();
        ~Test();
        void Destroy();
        void Start();
        int Init();
    private:
        GLRenderer*         m_Renderer;
        GameScene*          m_CurrentScene ;
        PerspCamera*        m_Camera ;
    };
};
#endif // _RGP_TEST_SAMPLE_H_
