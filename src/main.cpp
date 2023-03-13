#include "app/Application.h"
#include "app/Router.h"

int main(int argc, char** argv) {
    Application app;
    app.setRouter(std::make_unique<Router>());
    return app.run(argc, argv);
}