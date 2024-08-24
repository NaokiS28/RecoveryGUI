# BemaniUX

BemaniUX is a GUI menu system based off the [573in1 Tool](https://www.github.com/spicyjpeg/573in1) by [@spicyJPEG](https://www.github.com/spicyjpeg).

The primary goal of BemaniUX is to allow a developer to make a GUI version of their application (or to develop new ones) with a standardised and themeable menu system. The eventual goal is to have a cross-platform application which can allow developers to make versions of applications which can run directly from arcade boards, be it bespoke systems like the 573 or PC based systems.

The current version targets Windows XP as a base and uses the Windows GDI framework to draw to the screen to make it the most compatible with the existing code base and the componentised Windows Embedded platform. 

**BemaniUX IS EXPERIMENTAL AND NOT FULLY TESTED. 
Use at your own risk!**

##### Supported OS versions:
* Windows XP and up
* Windows PE (Windows 10 based and up)
* WINE (probably)

## Limitations
Please see the [limitations and known issues](issues.md) document for up to date and known about limitations/issue with this software.

## Contributing

Pull requests are welcome, especially if it helps with our planned [roadmap](roadmap.md)! If you wish to add functionality, feel free to do so, however please stick to the following guidelines:

* This software is designed for running on real arcade cabinets and where possible compatibility should be kept with this baseline. This means targeting ideally Windows XP/Embedded. Where possible, use Windows GDI. Else, make provisions for when the target cannot support GDI+ or DirectDraw e.t.c.
* The UI is designed around a baseline resolution of 320x240(QVGA) as to maintain compatibility with 573in1's theme structure. Whilst this gives an unintended but welcome retro-feel to the app, please try to keep this in mind when developing the app or themes and use the GPU scaling feature where possible on platforms with a higher base resolution.
* Do not include any code lifted as-is or minimally modified from a game disassembly. Rewritten and properly explained/commented code is fine.
* Do not include any game files or excerpts thereof. If necessary, only include metadata about the files such as hashes, offsets, sizes and so on.
* Store any such metadata as a file in the resource archive (see resources.json) rather than hardcoding it. If possible, provide a copy of the scripts used to gather the data from the original files in the tools directory.
* Adding a section to the documentation covering usage of the newly added functionality is not required, but would be highly appreciated.
* Please provide appropriate comments for any functions and its usage where possible and it is not already obvious from the code.

If you have any questions or doubts, or want to propose new features, feel free to reach out to the authors by opening an issue or through one of the Discord servers linked at the end of the page.

## License

BemaniUX is licensed under the GNU GPLv3. You may freely distribute modified versions as long as you do not relicense them and provide the full source code, attribution and a link back to this repository.

Some files are adapted from other projects or are third-party libraries and are more permissively licensed as a result. Refer to the license information at the top of each file for more details.

Known differences in licenses:
* stb_image is licenses under the MIT License.

