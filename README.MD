# OpenMPT ANSI Syntax Highlighting

* [Java version](https://github.com/cs127/mptsh-java)
* [Python version](https://github.com/cs127/mptsh-python)
* C++ version

## Info

This program adds syntax highlighting to your clipboard (using ANSI color codes) when copying pattern data in [OpenMPT](https://openmpt.org).

The main purpose of this program is to paste syntax-highlighted OpenMPT pattern data in [Discord](https://discord.com), using `ansi` Markdown code blocks.

## Demo

Discord screenshot, showing a section of an OpenMPT pattern, with and without syntax highlighting.
![Screenshot](ASSETS/DEMO.PNG)

## Command-line usage

`[EXEC] [OPTIONS] [COLORS]`

Options:
* `-h` or `--help`: Help (show all available options)
* `-i` or `--stdin`: Read input from STDIN instead of clipboard
* `-o` or `--stdout`: Write output to STDOUT instead of clipboard
* `-d` or `--markdown`: Wrap output in `ansi` Markdown code block (for Discord)
* `-r` or `--reverse`: Reverse mode (removes syntax highlighting instead of adding)

Using `-d` (`--markdown`) does nothing if `-r` (`--reverse`) is also enabled.

Colors:
`X,X,X,X,X,X,X,X` - Each value from 0 to 15 (Discord only supports 0 to 7)\
format: `Default,Note,Instrument,Volume,Panning,Pitch,Global,ChannelSeparator`\
if not provided: `7,5,4,2,6,3,1,7`

## Example color schemes

<table>
    <tr>
        <th rowspan="2">Color scheme</th>
        <th colspan="3">Preview</th>
        <th rowspan="2">Command-line argument</th>
    </tr>
    <tr>
        <th>Console (dark)</th>
        <th>Console (bright)</th>
        <th>Discord (dark)</th>
    </tr>
    <tr>
        <td>Default</td>
        <td><img src="ASSETS/COLOR/DEF_0.PNG"/></td>
        <td><img src="ASSETS/COLOR/DEF_1.PNG"/></td>
        <td><img src="ASSETS/COLOR/DEF_DIS.PNG"/></td>
        <td>
            Dark:<br/><code>7,5,4,2,6,3,1,7</code><br/>
            Bright:<br/><code>15,13,12,10,14,11,9,15</code>
        </td>
    </tr>
    <tr>
        <td>OpenMPT</td>
        <td><img src="ASSETS/COLOR/MPT_0.PNG"/></td>
        <td><img src="ASSETS/COLOR/MPT_1.PNG"/></td>
        <td><img src="ASSETS/COLOR/MPT_DIS.PNG"/></td>
        <td>
            Dark:<br/><code>7,4,6,2,6,3,1,7</code><br/>
            Bright:<br/><code>15,12,14,10,14,11,9,15</code>
        </td>
    </tr>
    <tr>
        <td>Impulse Tracker</td>
        <td><img src="ASSETS/COLOR/IT_0.PNG"/></td>
        <td><img src="ASSETS/COLOR/IT_1.PNG"/></td>
        <td><img src="ASSETS/COLOR/IT_DIS.PNG"/></td>
        <td>
            Dark:<br/><code>2,2,2,6,7,2,2,3</code><br/>
            Bright:<br/><code>10,10,10,14,15,10,10,11</code>
        </td>
    </tr>
    <tr>
        <td>FastTracker 2</td>
        <td><img src="ASSETS/COLOR/FT2_0.PNG"/></td>
        <td><img src="ASSETS/COLOR/FT2_1.PNG"/></td>
        <td><img src="ASSETS/COLOR/FT2_DIS.PNG"/></td>
        <td>
            Dark:<br/><code>3,3,3,3,3,3,3,6</code><br/>
            Bright:<br/><code>11,11,11,11,11,11,11,14</code>
        </td>
    </tr>
</table>
