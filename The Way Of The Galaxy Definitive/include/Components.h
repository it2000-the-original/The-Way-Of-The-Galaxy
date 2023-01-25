#pragma once

/* This is an header to include every component in one header
 * you must include every component in this file and never
 * include a single component without include Components.h. */

#include "ECS.h"
#include "PositionComponent.h"  // The component that define the position int the render area.
#include "SpriteComponent.h"    // The component that define the image to render of a entity.
#include "TextComponent.h"      // An alternative to SpriteComponent to define a text instad of an image.
#include "BulletComponent.h"    // The component to use for the bullets that destroy the entity when it goes out from the render area.
#include "ColliderComponent.h"  // The component that contain all of the parameters to use when the entity is in collision to another.
#include "ExplosionComponent.h" // The component to use for the explosions, created to show and animate the explosions and to destroy the entity after a specific time.
#include "EnemyComponent.h"     // A not definitive component created to destroy the entity of the enemies when they goes out to the render area.
#include "ExplodeComponent.h"   // The component to use to allows the component to explode and to define the size, the duration and the texture of the explosion.
#include "MissileComponent.h"   // The component to use for the missiles, created to accelerate the entity and contain the damage to subtract to the hitted entity.
#include "PlayerComponent.h"    // A specific component created to define all elements of the player entity.
#include "KeyboardController.h" // The component to use to controll the movements of an entity with w-s-d-a or with up-down-right-left arrows.