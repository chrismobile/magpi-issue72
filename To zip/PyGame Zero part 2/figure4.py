def on_mouse_down(pos):
    global myButtons
    for b in myButtons:
        if b.collidepoint(pos): b.state = True

def on_mouse_up(pos):
    global myButtons
    for b in myButtons: b.state = False

