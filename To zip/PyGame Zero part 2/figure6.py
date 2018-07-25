def update(): # Pygame Zero update function
    global myButtons, playingAnimation, playPosition
    if playingAnimation:
        playPosition += 1
        listpos = math.floor(playPosition/LOOPDELAY)
        if listpos == len(buttonList):
            playingAnimation = False
            clearButtons()
        else:   
            litButton = buttonList[listpos]
            if playPosition%LOOPDELAY > LOOPDELAY/2: litButton = -1
            bcount = 0
            for b in myButtons:
                if litButton == bcount: b.state = True
                else: b.state = False
                bcount += 1
