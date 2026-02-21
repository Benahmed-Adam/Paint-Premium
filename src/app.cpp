#include "../headers/app.hpp"
#include "../headers/tools.hpp"

// Constructeur
App::App(const sf::VideoMode& size, const std::string& title) 
    : m_window(size, title, sf::Style::Titlebar | sf::Style::Close), save_button(this->m_window, "Save", sf::Vector2f(60, 20), 14, sf::Vector2f(160, 22)), r(this->m_window, "r", sf::Vector2f(60, 20), 14, sf::Vector2f(260, 22), 3), g(this->m_window, "g", sf::Vector2f(60, 20), 14, sf::Vector2f(360, 22), 3), b(this->m_window, "b", sf::Vector2f(60, 20), 14, sf::Vector2f(460, 22), 3), save_color(this->m_window, "Save Color", sf::Vector2f(100, 20), 14, sf::Vector2f(560, 22)), erase_button(this->m_window, "erase", sf::Vector2f(60, 20), 14, sf::Vector2f(660, 22)) {

    //On définit la limite pour les FPS
    this->m_window.setFramerateLimit(60);

    //-------------------------------------------//

    //Création de la brosse
    this->brush.setRadius(this->brush_size);
    this->brush.setFillColor(current_color);
    this->brush.setOutlineColor(sf::Color::White);
    this->brush.setOutlineThickness(5);

    //-------------------------------------------//

    //Chargement et création de la police pour l'affichage des FPS
    if (!this->font.loadFromFile("ressources/arial.ttf"))
    {
        std::exit(-1);
    }

    this->text.setFont(this->font);
    this->text.setCharacterSize(20);
    this->text.setFillColor(sf::Color::White);
    this->text.setPosition(10, 10);

    //-------------------------------------------//

    //Ligne horizontale allant de x = 0 jusqu'a x = 800 sur y = 40
    this->line = sf::VertexArray(sf::Lines, 2);
    line[0].position = sf::Vector2f(0, 40);
    line[1].position = sf::Vector2f(800, 40);

    line[0].color = sf::Color::Red;
    line[1].color = sf::Color::Blue;

    //-------------------------------------------//

    //Ca évite d'avoir des erreurs car last_mouse_pos est initialisé avec (0;0)
    this->last_mouse_pos = sf::Mouse::getPosition();
}

//Gestion des évènements globaux
void App::processEvents() {

    sf::Event event;
    while (this->m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            this->m_window.close();
        //On voit si on a toucher a la molette afin de changer la taille de la brosse
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0) {
                this->brush_size++;
            }
            else if (event.mouseWheelScroll.delta < 0) {
                if (this->brush_size > 1) {
                    this->brush_size--;
                }
            }
            this->brush.setRadius(this->brush_size);
            this->brush.setOrigin(this->brush_size, this->brush_size);
        }

        //Ctrl + z afin d'éffacer le dernier point ajouter au vecteur points
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.control && event.key.code == sf::Keyboard::Z) {
                if (!this->points.empty()) {
                    this->points.pop_back();
                }
            }
        }

        //Event des éléments de l'UI tel que les zones de texte ou les boutons..
        this->save_button.handleEvent(event);
        this->save_color.handleEvent(event);
        this->erase_button.handleEvent(event);
        this->r.handleEvent(event);
        this->g.handleEvent(event);
        this->b.handleEvent(event);
    }
}

//Le coeur même de l'application c'est là que l'on gère toute la logique
void App::update() {

    //On vérifie si le bouton pour sauvegarder a été appuyé si oui on affiche une fenetre afin de sauvegarder un screenshot de la fenêtre
    if (this->save_button.isButtonClicked()) {
        this->save_button.desactivateButton();


        std::string location = showSaveFileDialog();
        this->render(false);
        sf::Texture texture;
        texture.create(this->m_window.getSize().x, this->m_window.getSize().y);
        std::cout << texture.getSize().x << " " << texture.getSize().y;

        sf::Image screenshot = texture.copyToImage();
        
        if (!screenshot.saveToFile(location)) {
            std::cerr << "Échec de l'enregistrement de la capture." << std::endl;
        }
    }

    //On vérifie si le bouton pour sauvegarder la couleur a été appuyé si oui on vérifie que la couleur ne dépasse pas 255 sinon on la met a 255 puis on change la couleur de la brosse (current_color)
    if (this->save_color.isButtonClicked()) {
        this->save_color.desactivateButton();

        int red = std::stoi(r.get());
        int green = std::stoi(g.get());
        int blue = std::stoi(b.get());

        if (red > 255) {
            this->r.text = "255";
            red = 255;
        }
        if (green > 255) {
            this->g.text = "255";
            green = 255;
        }
        if (blue > 255) {
            this->b.text = "255";
            blue = 255;
        }
        this->current_color = sf::Color(red, green, blue);
        this->brush.setFillColor(current_color);
    }

    //On vérifie si le bouton pour tout éffacer a été pressé si oui on supprime tout les éléments du vecteur points
    if (this->erase_button.isButtonClicked()) {
        this->erase_button.desactivateButton();

        this->points = {};
    }

    //-------------------------------------------//
    

    //On initialise ces variables afin d'éviter d'appeler sf::Mouse::getPosition trop de fois
    int mouse_pos_x = sf::Mouse::getPosition(this->m_window).x;
    int mouse_pos_y = sf::Mouse::getPosition(this->m_window).y;

    //On actualise la position de la brossse en fonction de la position de la souris
    brush.setPosition(static_cast<float>(mouse_pos_x), static_cast<float>(mouse_pos_y));

    //On vérifie si le clique gauche de la souris a été pressé et que la souris se situe dans la fenêtre et on vérifie aussi qu'elle est pas dans la zone de l'UI d'où le 40
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_pos_y > 40 && mouse_pos_y < 600 && mouse_pos_x < 800 && mouse_pos_x > 0) {
        bool same = false;
        
        //Boucle pour vérifier si la souris ne se situe pas sur le même cercle pour éviter de dessiner cercles sur cercles et de saturer la mémoire
        for (const auto& point : this->points) {
            int pointx = static_cast<int>(point.getPosition().x + point.getRadius());
            int pointy = static_cast<int>(point.getPosition().y + point.getRadius());

            if (pointx == mouse_pos_x && pointy == mouse_pos_y && point.getFillColor() == this->current_color && point.getRadius() == this->brush_size) {
                same = true;
                break;
            }
        }

        //Si la souris n'est pas ax coordonée d'un même cercle alors on peut commencer a ajouter les cercles a notre vecteur afin de les afficher
        if (!same) {
            //On calcule la distance entre l'ancienne position de souris et la nouvelle
            float distance = std::sqrt(std::pow(mouse_pos_x - this->last_mouse_pos.x, 2) + std::pow(mouse_pos_y - this->last_mouse_pos.y, 2));

            //On divise la taille de la brosse par 2 afin d'avoir une distance minimale
            float minDistance = this->brush_size / 2.0f;
            if (distance < minDistance) {
                distance = minDistance;
            }

            //On estime le nombre de cercles a ajouter entre (x1, y1) et (x2, y2) en divisant la distance par la taille de la brosse
            int num_circles = static_cast<int>(distance) / static_cast<int>(this->brush_size);

            //Afin d'éviter que rien ne se passe
            if (num_circles == 0) {
                num_circles = 1;
            }

            //On ajoute enfin nos cercles a notre vecteur points en calculant les coordonées (vx, vy) de ces derniers 
            for (int i = 0; i < num_circles; i++) {
                float t = static_cast<float>(i) / num_circles;
                float vx = this->last_mouse_pos.x * (1 - t) + mouse_pos_x * t;
                float vy = this->last_mouse_pos.y * (1 - t) + mouse_pos_y * t;
                sf::CircleShape point(this->brush_size);
                point.setFillColor(this->current_color);
                point.setPosition(static_cast<float>(vx) - this->brush_size, static_cast<float>(vy) - this->brush_size);
                this->points.push_back(point);
            }
            this->last_mouse_pos = { mouse_pos_x, mouse_pos_y };
            std::cout << this->points.size() << std::endl;
        }

    }

    //-------------------------------------------//

    //On vérifie si le bouton droit de la souris a été pressé
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

        //On itère a travers le vecteur points afin de savoir si un cercle se situe dans la brosse, si oui on le supprime du vecteur
        for (auto it = this->points.begin(); it != this->points.end();) {

            sf::Vector2f center1 = it->getPosition() + sf::Vector2f(it->getRadius(), it->getRadius());
            sf::Vector2f center2 = this->brush.getPosition() + sf::Vector2f(this->brush.getRadius(), this->brush.getRadius());

            float distance = std::sqrt(std::pow(center2.x - center1.x, 2) + std::pow(center2.y - center1.y, 2));
            float radiusSum = it->getRadius() + this->brush.getRadius();

            if (distance <= radiusSum) {
                it = this->points.erase(it);
            }
            else {
                ++it;
            }
        }
    }
    //On actualise l'ancienne position de la souris avec la nouvelle
    this->last_mouse_pos = { mouse_pos_x, mouse_pos_y };
}

//Affichage des cercles et des élements de l'UI (boutons, zone de texte...)
void App::render(bool ui = true) {
    this->m_window.clear(sf::Color::Black);
    for (const auto& point : this->points) {
        this->m_window.draw(point);
    }
    //Si le curseur est dans le zone de l'UI, alors on ne l'affiche pas (c plus clean comme ca)
    if (this->last_mouse_pos.y > 40) this->m_window.draw(brush);
    if (ui) {
        this->save_button.draw();
        this->save_color.draw();
        this->erase_button.draw();
        this->r.draw();
        this->g.draw();
        this->b.draw();
        this->m_window.draw(this->line);
        this->m_window.draw(this->text);
    }
    this->m_window.display();
}

//Une méthode permettant d'exécuter toutes les autres tant que la fenêtre n'est pas fermée
void App::run() {
    while (this->m_window.isOpen()) {
        this->processEvents();
        this->update();
        this->render();
        this->processFPS();
    }
}

//Une méthode qui permet simplement de calculer les FPS et de mettre a jour le texte des FPS
void App::processFPS() {
    if (this->show_fps) {
        this->frameCount++;
        this->elapsed = this->clock.getElapsedTime();
        this->fps = this->frameCount / this->elapsed.asSeconds();
        this->frameCount = 0;
        this->clock.restart();
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(0) << fps;

        std::string texte = "FPS : " + stream.str();
        this->text.setString(texte);
    }
}