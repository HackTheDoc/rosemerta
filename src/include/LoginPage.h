#pragma once

#include "Page.h"
#include "UILabel.h"
#include "UIButton.h"
#include "InputField.h"
#include "HyperLink.h"

class LoginPage : public Page {
public:
    LoginPage();
    ~LoginPage();

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
    UIButton* button;
    HyperLink* link;
};
