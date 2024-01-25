#ifndef INPUT_H
#define INPUT_H

enum class ButtonEventType {
    ButtonPressed,
    ButtonReleased,
    ButtonHeld,
    ButtonHeld300Ms,
    ButtonDoubleTap
};

#endif // INPUT_H