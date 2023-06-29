#pragma once

#include <SDL2/SDL.h>

#include "Page.h"
#include "UIButton.h"
#include "InputField.h"
#include "ImageDisplayer.h"
#include "DateInput.h"

class IdentityEditorPage : public Page {
public:
    IdentityEditorPage();
    ~IdentityEditorPage();

    void init() override;
    void update() override;
    void render() override;
    void destroy() override;

private:
    ImageDisplayer* profilePicture;
    InputField* usernameInput;
    InputField* ageInput;
    DateInput* birthdayInput;
    UIButton* saveButton;
};
