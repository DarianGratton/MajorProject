#pragma once

#include <box2d/box2d.h>

#include "../CScript.h"

class ContactListener : public b2ContactListener {

    void BeginContact(b2Contact* contact) override {

        b2BodyUserData bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (bodyUserData.pointer) {
            CScript* scriptClass = reinterpret_cast<CScript*>(bodyUserData.pointer);
            if (scriptClass)
                scriptClass->beginContact();

        }

        bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
        if (bodyUserData.pointer) {
            CScript* scriptClass = reinterpret_cast<CScript*>(bodyUserData.pointer);
            if (scriptClass)
                scriptClass->beginContact();
        }

    }

    void EndContact(b2Contact* contact) override {
        b2BodyUserData bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (bodyUserData.pointer) {
            CScript* scriptClass = reinterpret_cast<CScript*>(bodyUserData.pointer);
            if (scriptClass)
                scriptClass->endContact();

        }

        bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
        if (bodyUserData.pointer) {
            CScript* scriptClass = reinterpret_cast<CScript*>(bodyUserData.pointer);
            if (scriptClass)
                scriptClass->endContact();

        }   
    }

};