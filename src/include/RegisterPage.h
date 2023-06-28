#pragma once

#include "Page.h"
#include "UILabel.h"
#include "UIButton.h"
#include "InputField.h"

class RegisterPage : public Page {
public:
    RegisterPage();
    ~RegisterPage();

    void init() override;
    void update() override;
    void render() override;
    void destroy() override;
    void valid() override;

private:
    SDL_Rect border;
    UILabel* title;
    InputField* usernameInput;
    InputField* passwordInput;
    InputField* confirmPasswordInput;
    UIButton* button;
};
