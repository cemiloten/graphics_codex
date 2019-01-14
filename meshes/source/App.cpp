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
    settings.dataDir           = FileSystem::currentDirectory();

    return App(settings).run();
}

void App::makeCylinder() const {
    debugPrintf(format("%d\n", m_resolution).c_str());
    debugPrintf(format("%f\n", m_radius).c_str());
    debugPrintf(format("%f\n", m_height).c_str());
    

    IndexedTriangleList mesh;
    Array<Point3>& vertices(mesh.vertexArray);
    Array<int>& indices(mesh.indexArray);
    
    double thetha(2.0 * pi() / m_resolution);

    // Fill vertices
    for (int i = 0; i < 2 * m_resolution; ++i) {
        double angle((i - i % 2) / 2 * thetha);
        Vector3 vert(
            m_radius * sin(angle),
            m_height * (i % 2),
            m_radius * cos(angle));
        vertices.append(vert);
    }

    // Fill triangles
    for (int i = 0; i < 2 * m_resolution; i += 2) {
        int current_bot(i);
        int next_bot((i + 2) % vertices.size());
        int current_top((i + 1) % vertices.size());
        int next_top((i + 3) % vertices.size());
        
        indices.append(current_bot, next_bot, next_top);
        indices.append(next_top, current_top, current_bot);

        if (i >= 2 && i < 2 * m_resolution - 1) {
            indices.append(next_bot, current_bot, 0);
            indices.append(1, current_top, next_top);
        }
    }

    mesh.saveAsOff("model/cylinder.off");
}

App::App(const GApp::Settings& settings) : GApp(settings) {
    m_resolution = 5;
    m_radius = 2.0f;
    m_height = 3.0f;
}


// Called before the application loop begins.  Load data here and
// not in the constructor so that common exceptions will be
// automatically caught.
void App::onInit() {
    GApp::onInit();

    debugPrintf("Target frame rate = %f Hz\n", 1.0f / realTimeTargetDuration());
    setFrameDuration(1.0f / 60.0f);

    // Call setScene(shared_ptr<Scene>()) or setScene(MyScene::create()) to replace
    // the default scene here.
    
    showRenderingStats = true;

    makeGUI();
    loadScene(System::findDataFile("cylinder.Scene.Any"));
}


void App::makeGUI() {
    debugWindow->setVisible(true);
    developerWindow->videoRecordDialog->setEnabled(true);
    
    GuiPane* cylinderPane = debugPane->addPane("Make cylinder settings");
    cylinderPane->setNewChildSize(240);
    cylinderPane->addNumberBox("Resolution", &m_resolution);
    cylinderPane->addNumberBox("Radius", &m_radius);
    cylinderPane->addNumberBox("Height", &m_height);
    
    cylinderPane->addButton("Generate", [this]() {
        const shared_ptr<Model>& cylinderModel = scene()->modelTable()["cylinderModel"].resolve();
        scene()->removeModel(cylinderModel->name());
        
        App::makeCylinder();
        // TODO: add model from file to scene
        // cylinderModel = ArticulatedModel::fromFile("model/cylinder.off");
        scene()->insert(cylinderModel);

        shared_ptr<Entity> cylinder = scene()->entity("cylinder");
        
        // remove cylinder entity which has the wrong type
        if (notNull(cylinder) && isNull(dynamic_pointer_cast<VisibleEntity>(cylinder))) {
            scene()->remove(cylinder);
            cylinder.reset();
        }
        
        // entity does not exist
        if (isNull(cylinder)) {
            cylinder = scene()->createEntity(
                "cylinder",
                PARSE_ANY(VisibleEntity { model = "cylinderModel"; };));
        }
        else {
            dynamic_pointer_cast<VisibleEntity>(cylinder)->setModel(cylinderModel);
        }
    });

    debugWindow->pack();
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
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