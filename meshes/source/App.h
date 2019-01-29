/**
  \file App.h

  The G3D 10.00 default starter app is configured for OpenGL 4.1 and
  relatively recent GPUs.
 */
#pragma once
#include <G3D/G3D.h>

/** \brief Application framework. adding something for test. */
class App : public GApp {
protected:
    int m_cylinderResolution;
    float m_cylinderRadius;
    float m_cylinderHeight;

    float m_heightfieldYScale;
    float m_heightfieldXZScale;
    String m_heightfieldSource;
    
    int m_glassResolution;
    Array<Vector2> m_contour;
    String m_glassImage;
    
    shared_ptr<Model> m_model;

    enum ModelType {
        CYLINDER,
        HEIGHTFIELD,
        GLASS
    };

    //char* toStr(ModelType mtype) {
    //    switch (int(mtype)) {
    //        case CYLINDER:    return "Cylinder";
    //        case HEIGHTFIELD: return "Heightfield";
    //        case GLASS:     return "Glass".c_str();
    //        default:          return "None";
    //    }
    //}


    shared_ptr<Model> makeModel(ModelType mtype);

    void makeCylinder(
            Array<CPUVertexArray::Vertex>& vertexArray,
            Array<int>& indexArray);

    void makeHeightfield(
            Array<CPUVertexArray::Vertex>& vertexArray,
            Array<int>& indexArray,
            shared_ptr<Image>& image);

    void makeGlass(
            Array<CPUVertexArray::Vertex>& vertexArray,
            Array<int>& indexArray);

    void addModelToScene();
    Array<Vector2> makeContourFromImage(shared_ptr<Image>& image);
    void saveAsOFF(shared_ptr<Model>& model);
    

    /** Called from onInit */
    void makeGUI();
    
public:
    App(const GApp::Settings& settings = GApp::Settings());

    virtual void onInit() override;
    virtual void onAI() override;
    virtual void onNetwork() override;
    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt) override;
    virtual void onPose(
        Array<shared_ptr<Surface>>& posed3D,
        Array<shared_ptr<Surface2D> >& posed2D) override;

    // You can override onGraphics if you want more control over the rendering loop.
    // virtual void onGraphics(RenderDevice* rd, Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual void onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) override;
    virtual void onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual bool onEvent(const GEvent& e) override;
    virtual void onUserInput(UserInput* ui) override;
    virtual void onCleanup() override;
};
