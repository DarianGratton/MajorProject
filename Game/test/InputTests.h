#pragma once

#include <gtest/gtest.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "../src/Input.h"

TEST (IsKeyPressed, NoKeyPressed) 
{
    EXPECT_FALSE(Input::Instance().IsKeyPressed(GLFW_KEY_A));
}

TEST (IsKeyPressed, DifferentKeyIsPressed) 
{
    Input::Instance().PressKey(GLFW_KEY_A);
    EXPECT_FALSE(Input::Instance().IsKeyPressed(GLFW_KEY_W));
}

TEST (IsKeyPressed, KeyIsPressed) 
{
    Input::Instance().PressKey(GLFW_KEY_A);
    EXPECT_TRUE(Input::Instance().IsKeyPressed(GLFW_KEY_A));
}

TEST (IsMousePressed, LeftMousePressed) 
{
    Input::Instance().Clear();
    Input::Instance().PressMouseButton(GLFW_MOUSE_BUTTON_LEFT);
    EXPECT_TRUE(Input::Instance().IsMousePressed(true));
}

TEST (IsMousePressed, NotLeftMousePressed) 
{
    Input::Instance().Clear();
    Input::Instance().PressMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
    EXPECT_FALSE(Input::Instance().IsMousePressed(true));
}

TEST (IsMousePressed, RightMousePressed) 
{
    Input::Instance().Clear();
    Input::Instance().PressMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
    EXPECT_TRUE(Input::Instance().IsMousePressed(false));
}

TEST (IsMousePressed, NotRightMousePressed) 
{
    Input::Instance().Clear();
    Input::Instance().PressMouseButton(GLFW_MOUSE_BUTTON_LEFT);
    EXPECT_FALSE(Input::Instance().IsMousePressed(false));
}

TEST (IsMousePressed, NoMousePressed) 
{
    Input::Instance().Clear();
    EXPECT_FALSE(Input::Instance().IsMousePressed(true));
    EXPECT_FALSE(Input::Instance().IsMousePressed(false));
}

TEST (GetCursorPosition, PosX) 
{
    Input::Instance().CursorPositionCallback(1, 3);
    EXPECT_EQ(Input::Instance().GetCursorPosition(true), 1);    
}

TEST (GetCursorPosition, PosY) 
{
    Input::Instance().CursorPositionCallback(1, 3);
    EXPECT_EQ(Input::Instance().GetCursorPosition(false), 3);    
}