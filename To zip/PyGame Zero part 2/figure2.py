import pgzrun

myButtons = []
myButtons.append(Actor('redunlit', bottomright=(400,270)))
myButtons.append(Actor('greenunlit',bottomleft=(400,270)))
myButtons.append(Actor('blueunlit',topright=(400,270)))
myButtons.append(Actor('yellowunlit',topleft=(400,270)))
playButton = Actor('play', pos=(400,540))

def draw(): # Pygame Zero draw function
    screen.fill((30, 10, 30))
    for b in myButtons: b.draw()
    playButton.draw()

pgzrun.go()
