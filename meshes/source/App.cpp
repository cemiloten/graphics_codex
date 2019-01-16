/** \file App.cpp */
#include "App.h"
#include "IndexedTriangleList.h"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

int main(int argc, const char* argv[]) {
    initGLG3D(G3DSpecification());

    GApp::Settings settings(argc, argv);

    // Some common resolutions:
    settings.window.caption    = "Cylinder demo";
    settings.window.width      = 1024;
    settings.window.height     = 768;
    settings.window.resizable  = true;

    return App(settings).run();
}

shared_ptr<Model> App::makeCylinder() {
    drawMessage("Making cylinder...");

    const shared_ptr<ArticulatedModel>& model = ArticulatedModel::createEmpty("cylinderModel");

    ArticulatedModel::Part*     part = model->addPart("root");
    ArticulatedModel::Geometry* geometry = model->addGeometry("geom");
    ArticulatedModel::Mesh*     mesh = model->addMesh("mesh", part, geometry);

    
    // Assign a material
    mesh->material = UniversalMaterial::create();

    Array<CPUVertexArray::Vertex>& vertexArray = geometry->cpuVertexArray.vertex;
    Array<int>& indexArray = mesh->cpuIndexArray;

    const double thetha(2.0 * pi() / m_resolution);

    // Fill vertices
    for (int i = 0; i < 2 * m_resolution; ++i) {
        const double angle((i - i % 2) / 2 * thetha);
        CPUVertexArray::Vertex& v = vertexArray.next();
        v.position = Vector3(
            m_radius * sin(angle),
            m_height * (i % 2),
            m_radius * cos(angle));
    }

    // Fill triangles
    for (int i = 0; i < 2 * m_resolution; i += 2) {
        int current_bot(i);
        int next_bot((i + 2) % vertexArray.size());
        int current_top((i + 1) % vertexArray.size());
        int next_top((i + 3) % vertexArray.size());

        indexArray.append(current_bot, next_bot, next_top);
        indexArray.append(next_top, current_top, current_bot);

        if (i >= 2 && i < 2 * m_resolution - 1) {
            indexArray.append(next_bot, current_bot, 0);
            indexArray.append(1, current_top, next_top);
        }
    }

    ArticulatedModel::CleanGeometrySettings geometrySettings;
    geometrySettings.allowVertexMerging = false;
    model->cleanGeometry(geometrySettings);

    return model;
}


void App::addCylinderToScene() {
    const shared_ptr<Model>& cylinderModel = makeCylinder();

    if (scene()->modelTable().containsKey(cylinderModel->name())) {
        scene()->removeModel(cylinderModel->name());
    }
    scene()->insert(cylinderModel);

    shared_ptr<Entity> cylinder = scene()->entity("cylinder");
    // remove cylinder entity with the wrong type if it exists
    if (notNull(cylinder) && isNull(dynamic_pointer_cast<VisibleEntity>(cylinder))) {
        scene()->remove(cylinder);
        cylinder.reset();
    }

    // entity does not exist
    if (isNull(cylinder)) {
        cylinder = scene()->createEntity(
            "cylinder",
            PARSE_ANY(VisibleEntity{ model = "cylinderModel"; };));
    }
    else {
        dynamic_pointer_cast<VisibleEntity>(cylinder)->setModel(cylinderModel);
    }
}


App::App(const GApp::Settings& settings) : GApp(settings) {
    m_resolution = 5;
    m_radius = 2.0f;
    m_height = 3.0f;

    m_heightfieldYScale = 1.0f;
    m_heightfieldXZScale = 2.0f;
    m_heightfieldSource = FileSystem::currentDirectory();
}


// Called before the application loop begins.  Load data here and
// not in the constructor so that common exceptions will be
// automatically caught.
void App::onInit() {
    GApp::onInit();

    debugPrintf("Target frame rate = %f Hz\n", 1.0f / realTimeTargetDuration());
    setFrameDuration(1.0f / 60.0f);
    
    makeGUI();
    debugPrintf(FileSystem::currentDirectory().c_str());
    debugPrintf("\n");

    //loadScene("G3D Simple Cornell Box");
    loadScene("Make cylinder");
}


void App::makeGUI() {
    debugWindow->setVisible(true);
    
    developerWindow->cameraControlWindow->setVisible(false);
    developerWindow->sceneEditorWindow->setVisible(false);

    GuiPane* cylinderPane = debugPane->addPane("Make cylinder settings");
    cylinderPane->setNewChildSize(240);
    cylinderPane->addNumberBox("Resolution", &m_resolution, "", GuiTheme::LINEAR_SLIDER, 3, 32)->setUnitsSize(30);
    cylinderPane->addNumberBox("Radius", &m_radius, "m", GuiTheme::LOG_SLIDER, 0.1f, 10.0f)->setUnitsSize(30);
    cylinderPane->addNumberBox("Height", &m_height, "m", GuiTheme::LOG_SLIDER, 0.1f, 10.0f)->setUnitsSize(30);
    
    cylinderPane->addButton("Generate", [this]() {
        addCylinderToScene();
    });

    GuiPane* heightfieldPane = debugPane->addPane("Heightfield");
    heightfieldPane->setNewChildSize(240);
    heightfieldPane->addNumberBox("Max Y", &m_heightfieldYScale, "m", GuiTheme::LOG_SLIDER, 0.0f, 100.0f)->setUnitsSize(30);
    heightfieldPane->addNumberBox("XZ Scale", &m_heightfieldXZScale, "m/px", GuiTheme::LOG_SLIDER, 0.001f, 10.0f)->setUnitsSize(30);
    heightfieldPane->beginRow();
        heightfieldPane->addTextBox("Input image", &m_heightfieldSource)->setWidth(210);
        heightfieldPane->addButton("...", [this]() {
            FileDialog::getFilename(m_heightfieldSource, "png", false);
        })->setWidth(30);
    heightfieldPane->endRow();
    heightfieldPane->addButton("Generate", [this]() {
        shared_ptr<Image> image;
        try {
            image = Image::fromFile(m_heightfieldSource);
            // heightfield generation here
        }
        catch (...) {
            msgBox("Unable to load the image.", m_heightfieldSource);
        }
    });
    
    debugWindow->pack();
    debugWindow->setRect(Rect2D::xywh(0, 0, debugWindow->rect().width(), debugWindow->rect().height()));
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

void App::onAI() {
    GApp::onAI();
    // Add non-simulation game logic and AI code here
}


void App::onNetwork() {
    GApp::onNetwork();
    // Poll net messages here
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


// This default implementation is a direct copy of GApp::onGraphics3D to make it easy
// for you to modify. If you aren't changing the hardware rendering strategy, you can
// delete this override entirely.
void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& allSurfaces) {
    if (!scene()) {
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


void App::onCleanup() {
    // Called after the application loop ends.  Place a majority of cleanup code
    // here instead of in the constructor so that exceptions can be caught.
}
