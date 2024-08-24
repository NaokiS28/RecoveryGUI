# Known issues and limitations
This software was designed as a port to Windows PE for use as a general GUI for recovery menu options. As a result there have been a few compromises made to support this end goal.

## Limitations
Below is a known list of limitations with this software:
* The UI is designed around a baseline resolution of 320x240(QVGA) as to maintain compatibility with BemaniUX's theme structure. Whilst this gives an unintended but welcome retro-feel to the app, please try to keep this in mind when developing the app or themes and use the GPU scaling feature where possible on platforms with a higher base resolution.
* Currently Windows only and only uses Windows GDI (due to original design goals)
* Not DPI aware, will result in blurry looking image on Hi-DPI displays
* Codebase is not super portable. Where possible, it has been abstracted to not rely on Windows specfic calls (ie replacing the gpu class should be enough to draw), but there's still many instances where this can be improved.

## Known issues/bugs
The following issues are known about and will be marked as fixed with check box.
- [ ] Window does not resize/redraw correctly
- [ ] GUI Scaling is entirely broken. It will scale primatives but not images or text properly.
- [ ] Input system is non-existant really
- [ ] Virtual/Mouse is non-existant.
- [ ] Virtual keyboard is non-existant.