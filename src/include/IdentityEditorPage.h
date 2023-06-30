#pragma once

#include <SDL2/SDL.h>

#include "Page.h"
#include "UIButton.h"
#include "ImageDisplayer.h"
#include "InputField.h"
#include "StatusSelector.h"
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
    StatusSelector* statusSelector;
    DateInput* birthdayInput;
    UIButton* saveButton;
};
