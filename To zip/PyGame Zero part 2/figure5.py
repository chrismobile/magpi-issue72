def playAnimation():
    global playPosition, playingAnimation
    playPosition = 0
    playingAnimation = True

def addButton():
    global buttonList
    buttonList.append(randint(0, 3))
    playAnimation()
