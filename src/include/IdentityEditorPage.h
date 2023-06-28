#pragma once

#include <SDL2/SDL.h>

#include "UIElement.h"
#include "UIButton.h"
#include "InputField.h"
#include "ImageDisplayer.h"

class IdentityEditorPage : public UIElement {
public:
    IdentityEditorPage();
    ~IdentityEditorPage();

    void update() override;
    void draw() override;
    void destroy() override;

private:
    ImageDisplayer* profilePicture;
    InputField* usernameInput;
    InputField* ageInput;
    UIButton* saveButton;
};
