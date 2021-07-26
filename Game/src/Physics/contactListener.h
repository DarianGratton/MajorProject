#pragma once

#include <entityx/entityx.h>
#include <box2d/box2d.h>

#include "../CScript.h"
#include "../components.h"

using namespace entityx;

class ContactListener : public b2ContactListener {

    void BeginContact(b2Contact* contact) override {

        // Get user data
        b2BodyUserData bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        b2BodyUserData bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

        if (!bodyUserDataA.pointer || !bodyUserDataB.pointer) {
            LOG_TRACE("No data");
            return;
        }

        // Get entities from data
        Entity* entityA = reinterpret_cast<Entity*>(bodyUserDataA.pointer);
        Entity* entityB = reinterpret_cast<Entity*>(bodyUserDataB.pointer);

        // Call fixture A's contact function
        if (entityA->has_component<Script>()) {
            ComponentHandle<Script> entityAScript = entityA->component<Script>();
            CScript* scriptClass = reinterpret_cast<CScript*>(entityAScript.get()->script);
            if (scriptClass)
                scriptClass->beginContact(entityA, entityB);
        }

        // Call fixture B's contact function
        if (entityB->has_component<Script>()) {
            ComponentHandle<Script> entityBScript = entityB->component<Script>();
            CScript* scriptClass = reinterpret_cast<CScript*>(entityBScript.get()->script);
            if (scriptClass)
                scriptClass->beginContact(entityB, entityA);
        }

    }

    void EndContact(b2Contact* contact) override {
        // Get user data
        b2BodyUserData bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        b2BodyUserData bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

        if (!bodyUserDataA.pointer || !bodyUserDataB.pointer) {
            LOG_TRACE("No data");
            return;
        }

        // Get entities from data
        Entity* entityA = reinterpret_cast<Entity*>(bodyUserDataA.pointer);
        Entity* entityB = reinterpret_cast<Entity*>(bodyUserDataB.pointer);

        // Call fixture A's contact function
        if (entityA->has_component<Script>()) {
            ComponentHandle<Script> entityAScript = entityA->component<Script>();
            CScript* scriptClass = reinterpret_cast<CScript*>(entityAScript.get()->script);
            if (scriptClass)
                scriptClass->endContact(entityA, entityB);
        }

        // Call fixture B's contact function
        if (entityB->has_component<Script>()) {
            ComponentHandle<Script> entityBScript = entityB->component<Script>();
            CScript* scriptClass = reinterpret_cast<CScript*>(entityBScript.get()->script);
            if (scriptClass)
                scriptClass->endContact(entityB, entityA);
        }
    }

};