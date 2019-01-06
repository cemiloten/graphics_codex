/** \file App.cpp */
#include "App.h"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

void writeStaircaseScene() {
    TextOutput to("staircase.Scene.Any");
    to.writeSymbol('{');
    to.writeNewline();
    to.pushIndent();

    to.writeSymbols("name = \"Staircase\";\n\n");
    
    to.writeSymbols(
        "models = {\n"
        "   crateModel = ArticulatedModel::Specification {\n"
        "      filename", "=", "\"model/crate/crate.obj\";\n"
        "       preprocess = {\n"
        "           setMaterial(all(), \"material/roughcedar/roughcedar-lambertian.png\");\n"
        "           transformGeometry(all(), Matrix4::scale(0.05, 0.05, 1));\n"
        "       };\n"
        "   };\n"
        "};\n\n"

        "entities = {\n"
        "    camera = Camera {\n"
        "        frame = CFrame::fromXYZYPRDegrees(0.7, 2.8, 3.6, 9.7, -19.4, 0);\n"
        "    };\n\n"

        "    skybox = Skybox {\n"
        "        texture = 0.5;\n"
        "    };\n\n"

        "    sun = Light {\n"
        "        shadowsEnabled = true;\n"
        "        frame = CFrame::fromXYZYPRDegrees(0, 0, 0, 0, 0, 0);\n"
        "        shadowMapSize = Vector2int16(2048, 2048);\n"
        "        type = \"DIRECTIONAL\";\n"
        "    };\n\n"
    );

    for (int i = 0; i < 50; ++i) {
        to.writeSymbols(format("    crate%02d = VisibleEntity {\n", i));
        to.writeSymbols("   model = \"crateModel\";\n");
        to.writeSymbols(
            format(
                "   frame = CFrame::fromXYZYPRDegrees(0, %f, 0, %d, 0, 0);\n",
                i / double(20),
                i * 8));
        to.writeSymbols("};\n\n");
    }

    to.writeSymbol("    };\n};");
    to.commit();
}

void App::updateFinalScene() {
    int boxCount(40);
    double period;
    for (int i = 0; i < boxCount; ++i) {
        if (i < 10) {
            period = double(i) / 8.0;
        }
        else {
            period = double(i + pi()) / 8.0;
        }
        
        shared_ptr<VisibleEntity> v(
            VisibleEntity::create(
                format("cube%02d", i),
                scene().get(),
                scene()->modelTable()["cubeModel"].resolve(),
                CoordinateFrame(Point3(0, i, 0)))
        );
    
//        const Any& a = Any::parse(
//            format(STR(
//                timeShift(
//                    PhysicsFrameSpline {
//                        control = [
//                            CFrame::fromXYZYPRDegrees(0, %i, 0, 0, 0, 0),
//                            CFrame::fromXYZYPRDegrees(3, %i, 0, 0, 0, 0)
//                        ];
//
//                        time = [0, 2];
//
//                        extrapolationMode = CYCLIC;
//                        interpolationMode = CUBIC;
////                        finalInterval = AUTOMATIC;
//                    },
//                    %f
//                )
//            ),
//            i, i, double(i) / 4.0));

        const Any& a = Any::parse(
            format(STR(
                transform(
                    CFrame::fromXYZYPRDegrees(0, %d, 0, 0, 0, 0),
                    timeShift(orbit(3.0, 3.0), %f)
                )
            ),
            i % (boxCount / 2),
            period)
        );
        
        const shared_ptr<Entity::Track>& track(
            Entity::Track::create(v.get(), scene().get(), a));
        v->setTrack(track);
        
        v->setShouldBeSaved(false);
        scene()->insert(v);
    }
}

int main(int argc, const char* argv[]) {
    initGLG3D(G3DSpecification());

    GApp::Settings settings(argc, argv);

    // Some common resolutions:
//    settings.window.width  = 854;
//    settings.window.height = 480;
    settings.window.width  = 1024;
    settings.window.height = 768;

    settings.window.fullScreen          = false;
    settings.window.resizable           = ! settings.window.fullScreen;
    settings.window.framed              = ! settings.window.fullScreen;
    try {
        settings.window.defaultIconFilename = "icon.png";
    }
    catch (...) {
        debugPrintf("Could not find icon\n");
    }

    return App(settings).run();
}

App::App(const GApp::Settings& settings) : GApp(settings) {}


// Called before the application loop begins.  Load data here and
// not in the constructor so that common exceptions will be
// automatically caught.
void App::onInit() {
    GApp::onInit();

    debugPrintf("Target frame rate = %f Hz\n", 1.0f / realTimeTargetDuration());
    
    //const shared_ptr<Entity>& sphere = scene()->entity("Sphere");
    //sphere->setFrame(Point3(0.0f, 1.5f, 0.0f));
    
    setFrameDuration(1.0f / 60.0f);

    // Call setScene(shared_ptr<Scene>()) or setScene(MyScene::create()) to replace
    // the default scene here.
    
    showRenderingStats      = true;

    makeGUI();
    loadScene(System::findDataFile("final.Scene.Any"));
    updateFinalScene();
    
//    loadScene(
//#       ifndef G3D_DEBUG
//            "G3D Sponza"
//#       else
//            "G3D Simple Cornell Box (Area Light)" // Load something simple
//#       endif
//        //developerWindow->sceneEditorWindow->selectedSceneName()  // Load the first scene encountered
//        );
}


void App::makeGUI() {
    debugWindow->setVisible(false);
    developerWindow->videoRecordDialog->setEnabled(true);

    GuiPane* infoPane = debugPane->addPane("Info", GuiTheme::ORNATE_PANE_STYLE);
    
    // Example of how to add debugging controls
    infoPane->addLabel("You can add GUI controls");
    infoPane->addLabel("in App::onInit().");
    infoPane->addButton("Exit", [this]() { m_endProgram = true; });
    infoPane->pack();

    // More examples of debugging GUI controls:
    // debugPane->addCheckBox("Use explicit checking", &explicitCheck);
    // debugPane->addTextBox("Name", &myName);
    // debugPane->addNumberBox("height", &height, "m", GuiTheme::LINEAR_SLIDER, 1.0f, 2.5f);
    // button = debugPane->addButton("Run Simulator");
    // debugPane->addButton("Generate Heightfield", [this](){ generateHeightfield(); });
    // debugPane->addButton("Generate Heightfield", [this](){ makeHeightfield(imageName, scale, "model/heightfield.off"); });

    debugWindow->pack();
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
}


// This default implementation is a direct copy of GApp::onGraphics3D to make it easy
// for you to modify. If you aren't changing the hardware rendering strategy, you can
// delete this override entirely.
void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& allSurfaces) {
    if (! scene()) {
        if ((submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) && (!rd->swapBuffersAutomatically())) {
            swapBuffers();
        }
        rd->clear();
        rd->pushState(); {
            rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
            drawDebugShapes();
        } rd->popState();
        return;
    }


    GBuffer::Specification gbufferSpec = m_gbufferSpecification;
    extendGBufferSpecification(gbufferSpec);
    m_gbuffer->setSpecification(gbufferSpec);
    m_gbuffer->resize(m_framebuffer->width(), m_framebuffer->height());
    m_gbuffer->prepare(rd, activeCamera(), 0, -(float)previousSimTimeStep(), m_settings.hdrFramebuffer.depthGuardBandThickness, m_settings.hdrFramebuffer.colorGuardBandThickness);

    m_renderer->render(rd,
        activeCamera(),
        m_framebuffer,
        scene()->lightingEnvironment().ambientOcclusionSettings.enabled ? m_depthPeelFramebuffer : nullptr,
        scene()->lightingEnvironment(), m_gbuffer,
        allSurfaces);
   
    // Debug visualizations and post-process effects
    rd->pushState(m_framebuffer); {
        // Call to make the App show the output of debugDraw(...)
        rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
        drawDebugShapes();
        const shared_ptr<Entity>& selectedEntity = (notNull(developerWindow) && notNull(developerWindow->sceneEditorWindow)) ? developerWindow->sceneEditorWindow->selectedEntity() : nullptr;
        scene()->visualize(rd, selectedEntity, allSurfaces, sceneVisualizationSettings(), activeCamera());

        onPostProcessHDR3DEffects(rd);
    } rd->popState();

    // We're about to render to the actual back buffer, so swap the buffers now.
    // This call also allows the screenshot and video recording to capture the
    // previous frame just before it is displayed.
    if (submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) {
        swapBuffers();
    }

    // Clear the entire screen (needed even though we'll render over it, since
    // AFR uses clear() to detect that the buffer is not re-used.)
    rd->clear();

    // Perform gamma correction, bloom, and AA, and write to the native window frame buffer
    m_film->exposeAndRender(rd, activeCamera()->filmSettings(), m_framebuffer->texture(0), settings().hdrFramebuffer.colorGuardBandThickness.x + settings().hdrFramebuffer.depthGuardBandThickness.x, settings().hdrFramebuffer.depthGuardBandThickness.x, 
        Texture::opaqueBlackIfNull(notNull(m_gbuffer) ? m_gbuffer->texture(GBuffer::Field::SS_POSITION_CHANGE) : nullptr),
        activeCamera()->jitterMotion());
}


void App::onAI() {
    GApp::onAI();
    // Add non-simulation game logic and AI code here
}


void App::onNetwork() {
    GApp::onNetwork();
    // Poll net messages here
}


void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
    GApp::onSimulation(rdt, sdt, idt);

    // Example GUI dynamic layout code.  Resize the debugWindow to fill
    // the screen horizontally.
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
}


bool App::onEvent(const GEvent& event) {
    // Handle super-class events
    if (GApp::onEvent(event)) { return true; }

    // If you need to track individual UI events, manage them here.
    // Return true if you want to prevent other parts of the system
    // from observing this specific event.
    //
    // For example,
    // if ((event.type == GEventType::GUI_ACTION) && (event.gui.control == m_button)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == GKey::TAB)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) { ... return true; }

    if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) { 
        shared_ptr<DefaultRenderer> r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
        r->setDeferredShading(! r->deferredShading());
        return true; 
    }

    return false;
}


void App::onUserInput(UserInput* ui) {
    GApp::onUserInput(ui);
    (void)ui;
    // Add key handling here based on the keys currently held or
    // ones that changed in the last frame.
}


void App::onPose(Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) {
    GApp::onPose(surface, surface2D);

    // Append any models to the arrays that you want to later be rendered by onGraphics()
}


void App::onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& posed2D) {
    // Render 2D objects like Widgets.  These do not receive tone mapping or gamma correction.
    Surface2D::sortAndRender(rd, posed2D);
}


void App::onCleanup() {
    // Called after the application loop ends.  Place a majority of cleanup code
    // here instead of in the constructor so that exceptions can be caught.
}
