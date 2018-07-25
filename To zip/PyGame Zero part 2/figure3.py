import pgzrun

myButtons = []
myButtons.append(Actor('redunlit', bottomright=(400,270)))
myButtons[0].state = False
myButtons.append(Actor('greenunlit',bottomleft=(400,270)))
myButtons[1].state = False
myButtons.append(Actor('blueunlit',topright=(400,270)))
myButtons[2].state = False
myButtons.append(Actor('yellowunlit',topleft=(400,270)))
myButtons[3].state = False
buttonsLit = ['redlit', 'greenlit', 'bluelit', 'yellowlit']
buttonsUnlit = ['redunlit', 'greenunlit', 'blueunlit', 'yellowunlit']
playButton = Actor('play', pos=(400,540))

def draw(): # Pygame Zero draw function
    screen.fill((30, 10, 30))
    for b in myButtons: b.draw()
    playButton.draw()

def update(): # Pygame Zero update function
    bcount = 0
    for b in myButtons:
        if b.state == True: b.image = buttonsLit[bcount]
        else: b.image = buttonsUnlit[bcount]
        bcount += 1
        
pgzrun.go()
