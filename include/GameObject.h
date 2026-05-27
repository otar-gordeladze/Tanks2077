// GameObject.h - the abstract base class for everything in the game world
// All visible/interactive things (tanks, walls, bullets, bonuses) will inherit from this.
#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class GameObject
{
protected:
    // Position in window coordinates (pixels from top-left).
    // protected = subclasses can access this directly, but outside code cannot.
    sf::Vector2f position;

    // Whether this object is still alive. When false, Game will remove it.
    bool active = true;

public:
    // Constructor: every GameObject starts at a given position.
    GameObject(float x, float y) : position(x, y) {}

    // Virtual destructor - CRITICAL for polymorphism!
    // When we delete a base-class pointer, this ensures the correct derived
    // destructor runs. Without "virtual" here, derived destructors would be
    // skipped and we'd have memory leaks.
    virtual ~GameObject() = default;

    // Pure virtual functions (the "= 0" makes them pure).
    // Any subclass MUST implement both of these, or it can't be instantiated.
    // This is what makes GameObject abstract.

    // Update the object's state - called once per frame.
    // dt = "delta time" in seconds since last frame
    // objects = the Game's vector, in case this object needs to spawn new ones (e.g. a tank firing a bullet)
    virtual void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) = 0;

    // Draw the object to the window.
    virtual void draw(sf::RenderWindow& window) = 0;

    // Returns the axis-aligned bounding rectangle of this object.
    // Used for collision detection. Every subclass implements it.
    virtual sf::FloatRect getBounds() const = 0;

    // Helpers any subclass can use:
    void setActive(bool a) { active = a; }
    bool isActive() const { return active; }
    sf::Vector2f getPosition() const { return position; }
};
