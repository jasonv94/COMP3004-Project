COMP 3004 Final Project
December 9th, 2022
Team 22

Jason Varghese 101209461
Rida Saeed 101072436
Marie-Josee Salameh 101146303
Christopher Krol 101010495

Who did what in the project:

Jason Varghese:
- UI
- Database
- Program Logic
- Some diagrams

Rida Saeed:
- UI
- Diagrams

Marie-Josee Salameh:
- UI
- Program Logic
- Diagrams

Christopher Krol:
- Designed Classes
- Program Logic

Deliverable files:
- design documents pdf
- Resource folder containing PNG files for UI
- Qt files(COMP3004-Project.pro, dbmanager.h, history.h, mainwindow.h, menu.h, therapy.h user.h, dbmanager.cpp, history.cpp, mainwindow.cpp, menu.cpp, therapy.cpp user.cpp, main.cpp, mainwindow.ui)

Device instructions:

- press power button to turn on device
- select user
- click on new session, or on history to choose a recorded session if available
- click the therapy type button to choose therapy, click on the time menu with the select button, to choose time
- click the record to save
- click the check button to start the session
- using the up and down arrows the intensity can be adjusted
- the session ends once time is up, or if the power button is pressed, if the record option is selected it will save at that point
- if battery level becomes to low (~5) click the battery combo box when a current session isnt running to increase

Tested Scenarios:
passed:
- creating a new session with a set time
- creating a new session with a custom time
- running a session with the record option selected
- running a session with the record option not selected
- trying to start a session with out connection
- loosing connection during a session, causing it to pause then reconnecting it to resume the session
- trying to run a session on low battery
- battery running low during a session
- turning off the machine mid-session when record is selected, and still recording
- browsing and selecting a session from history
- running a selected session from history
- changing the intensity during a session
Troubleshooting scenarios:
- using buttons when the power is off
- using session buttons(alpha, beta1, theta, delta) when not in the new session menu
- using the clear all button when not in history
- using clear all button when history is empty
- trying to start a session when no in new session menu
- trying to start a session when not all of the session info is there(either length or session type missing)

failed:
