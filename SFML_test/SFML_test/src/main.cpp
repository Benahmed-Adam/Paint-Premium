#include "../headers/app.hpp"


int main() {

    App app(sf::VideoMode(800, 600), "logiciel de qualit�");
    app.show_fps = true;
    app.run();

    return 0;
}
