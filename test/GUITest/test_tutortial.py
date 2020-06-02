#!/usr/bin/python3
import pyautogui
import time
import datetime
import platform
import subprocess
import Xlib.display
import os
import argparse
import re

#Determine OS so we get the correct window manager
runningOS       = None
Ubuntu          = 'Ubuntu_18.04'
Darwin          = 'Darwin'
if ('Ubuntu' in platform.platform()) and ('18.04' in platform.platform()):
    runningOS = Ubuntu
elif ('Darwin' in platform.platform()):
    runningOS = Darwin
else:
    runningOS = None

assert (runningOS != None), "Unsupported OS - Darwin, Ubuntu 18.04 supported. found: " + platform.platform()

#Set up the parser
parser = argparse.ArgumentParser(description='GUI Test program for CoreGenPortal')
parser.add_argument('-l', '--headless',  action='store_true', default=False, required=False, help='Run headless using Xvfb, Ubuntu ONLY')
parser.add_argument('-p', '--portal_path', required=False,  default='/opt/coregen/bin/CoreGenPortal', help='Path to CoreGenPortal binary - overrides env var COREGENPORTAL_BIN')
parser.add_argument('-r', '--resolution', required=True, default='Auto', help='Resolution of screen in "X x Y" pixels where you are launching \
        CoreGenPortal "Auto" will direct the script to attempt to determine your screen resolution.  Multi-monitor environments may have issues')

args = parser.parse_args()

runningHeadless = args.headless             # when true you are running this python script using xvfb-run
assert( (runningHeadless and (runningOS is Ubuntu)) or (runningHeadless is False)), "Running headless supported only on Ubuntu"

screenRes = args.resolution
screenResX = 0
screenResY = 0
if('Auto' in screenRes):
    if(runningOS is Darwin):
        o   = subprocess.run(["system_profiler", "SPDisplaysDataType"], universal_newlines=True, stdout=subprocess.PIPE)  #using the Python 3.6 signature of this call
        s   = o.stdout.split()
        assert('Resolution:' in s), "Could not determine screen resolution from System Profiler"
        screenResX = int(s[s.index('Resolution:')+1])   
        screenResY = int(s[s.index('Resolution:')+3])
    if(runningOS is Ubuntu):
        o = subprocess.run("xdpyinfo",  universal_newlines=True, stdout=subprocess.PIPE)
        s = o.stdout.split()
        assert('dimensions:' in s), "Could not determine screen resolution from xdpyinfo"
        screenResX = int(s[s.index('dimensions:')+1].split('x')[0])
        screenResX = int(s[s.index('dimensions:')+1].split('x')[1])
else:
    screenResX = int(re.split('x | X', args.resolution)[0]) 
    screenResY = int(re.split('x | X', args.resolution)[1]) 

#Window offset constants
nbXDelta        = 0                         # X Distance "notebook" image is from window edge - does not include window banner
if(runningOS is Darwin):
    nbYDelta        = 50                    # Y Distance "notebook" image is from window edge - does not include window banner
elif(runningOS is Ubuntu):
    nbYDelta        = 56                    # Y Distance "notebook" image is from window edge - does not include window banner
irXDelta        = 312                       # X Distance "IR" image is from window edge - does not include window banner
irYDelta        = 54                        # Y Distance "IR" image is from window edge - does not include window banner
newProjDiagX    = (screenResX / 2)          # X Distance Text box in New project diag is from window edge
newProjDiagY    = (screenResY / 2) - 99     # Y Distance Text box in New project diag is from window top
newProjTypeX    = newProjDiagX              # Project Type drop-down box - X offset from window edge
newProjTypeY    = newProjDiagY + 57         # Project Type drop down box - Y offset from window top
newProjOKX      = newProjDiagX + 154        # New Project OK button location - relative to window edge
newProjOKY      = newProjDiagY + 150        # New Project OK button location - relative to window top
if(runningOS is Darwin):
    nodesX          = 50                    # "Nodes" tree root in Project Notebook->Modules location - relative to widow edge
    nodesY          = 105                   # "Nodes" tree root in Project Notebook->Modules location - relative to widow top
elif(runningOS is Ubuntu):
    nodesX          = 23                    # "Nodes" tree root in Project Notebook->Modules location - relative to widow edge
    nodesY          = 123                   # "Nodes" tree root in Project Notebook->Modules location - relative to widow top

cacheDiagCordX  = 355
cacheDiagCordY  = 220
cacheNameX      = 200                       # Cache Name text box offset from Cache diag screenshot
cacheOKX        = 50                        # New Cache Daig OK button.  Relative to cache diag screenshot
cacheOKY        = 290                       # New Cache Daig OK button.  Relative to cache diag screenshot

#ScreenshotFolder
folderName = None
if(runningOS is Darwin):
    folderName  = './Darwin/'
elif(runningOS is Ubuntu):
    folderName  = './Ubuntu/'


#Test variables
newProjName     = 'TestNewProj' + datetime.datetime.fromtimestamp(time.time()).strftime('%Y%m%d%H%M')
cacheName       = 'TestCache'
cacheSets       = '1'
cacheWays       = '1'
cacheLineSize   = '64'



pyautogui.PAUSE = 1     #Pause one second between all GUI commands - this lets diaglog boxes, etc to pop up

#launch CoreGenPortal
print("launching CoreGen... I'll wait 15 sec")
coreGenP = subprocess.Popen([os.getenv("COREGENPORTAL_BIN",args.portal_path),"--maximize"])
time.sleep(15)
assert(coreGenP.poll() == None), "Error launching CoreGenPortal"

#Locate known points on CoreGenGUI - we assume CoreGenGUI is full screen on startup.  Resolution determined above

if(runningOS is Darwin):
    windowTopX = 0
    windowTopY = 45
elif(runningOS is Ubuntu):
    windowTopX = 0
    windowTopY = 80
else:
    assert runningHeadless, "We are setting X,Y as though running headless"
    windowTopX = 0
    windowTopY = 0

assert((windowTopX >= 0) and (windowTopY >= 0)), "Window corner cannot be at a negative coordinate"

print("Found CoreGen at: (" + str(windowTopX) + "," + str(windowTopY) + ")" )

########################
#Create a new project
########################
if(runningOS is Darwin):
    pyautogui.click(1,1)
    pyautogui.press(['right', 'right', 'right', 'right', 'right', 'down', 'enter'])             # Select Build -> New Project
    pyautogui.click(x=(windowTopX + newProjDiagX), y=(windowTopY + newProjDiagY), clicks=2)     # Click in project name text box
    pyautogui.write(newProjName)                                                                # Name the project
    pyautogui.click(x=(windowTopX + newProjOKX), y=(windowTopY + newProjOKY))                   # Default project type is SoC - leave unchanged and click OK

elif(runningOS is Ubuntu):
    pyautogui.hotkey('alt', 'p')                                                                # Select Preferences from menu bar    
    pyautogui.press(['right', 'enter'])                                                         # Move Right one, then New Project is first meun item
    pyautogui.press(['tab'])                                                                    # Tab into new project dialog - also selects the project name
    pyautogui.write(newProjName)                                                                # Name the project
    pyautogui.press(['tab'])                                                                    # Tab over to OK button in dialog box
    pyautogui.press(['tab'])
    pyautogui.press(['enter']) 

print("Created a new project: " + newProjName)
######################
# Add a new cache node
######################
pyautogui.click(x=(windowTopX + nodesX), y=(windowTopY + nodesY))                           # Click "Nodes" at the root of the tree
pyautogui.press('right')                                                                    # Expand the tree
pyautogui.move(0,20)                                                                        # Move the cursor onto the "Cache" Node
pyautogui.click(button='right')                                                             # Right click to display 'Add Node'
pyautogui.move(5,5)
pyautogui.click(button='left')                                                              # Left click to select 'Add Node'

if(runningOS is Darwin):
    pyautogui.click( x=(cacheDiagCordX + cacheNameX), y=cacheDiagCordY)              # Click the cache name text box

print("Adding new cache: " + cacheName)
pyautogui.write(cacheName)                                                                  # Name the cache, then tab to sets, then tab to ways
pyautogui.press('tab')
pyautogui.write(cacheSets)
pyautogui.press('tab')
pyautogui.write(cacheWays)
pyautogui.press('tab')
pyautogui.write(cacheLineSize)
time.sleep(1)

if(runningOS is Darwin):
    pyautogui.click( x=(cacheDiagCordX + cacheOKX), y=(cacheDiagCordY + cacheOKY))   # Press Save
    time.sleep(1)
    pyautogui.hotkey('command', 's')                                                        # Save Project
    time.sleep(1)
    pyautogui.hotkey('command', 'q')                                                        # Quit Coregen
    time.sleep(1)
    pyautogui.press('enter')                                                                # Confirm quit diag box

elif(runningOS is Ubuntu):
    pyautogui.hotkey('alt', 's')                                                            # Save cache 
    time.sleep(1)
    pyautogui.hotkey('ctrl', 's')                                                           # Save Project
    time.sleep(1)
    pyautogui.hotkey('ctrl', 'q')                                                           # Quit Coregen
    time.sleep(1)
    pyautogui.press('enter')                                                                # Confirm quit diag box


time.sleep(1)
assert(coreGenP.poll() == 0), "WARNING: CoreGenPortal did not exit cleanly"

print("Test complete! CoreGenPortal returned: " + str(coreGenP.poll()))



