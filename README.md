# Battery Spacer Generator

A tool for generating 3D-printable battery cell spacers with customizable parameters. Create precise OpenSCAD files for both linear and slanted (hexagonal) battery pack layouts with integrated BMS wire routing holes.

![Battery Spacer Generator](https://img.shields.io/badge/OpenSCAD-Compatible-blue) ![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C) ![License](https://img.shields.io/badge/license-%20%20GNU%20GPLv3%20-green?style=plastic)

## Features

- **Linear & Slanted Layouts**: Support for both traditional linear arrangements and space-efficient hexagonal patterns
- **Configurable Parameters**: Customize cell dimensions, wall thickness, height, corner radius, and more
- **BMS Wire Integration**: Automatic generation of balancer wire holes positioned correctly for your pack configuration
- **Insulation Rings**: Built-in top insulation rings to prevent short circuits
- **Two Interfaces**: 
  - Command-line C++ scripts for local generation
  - Web-based GUI for easy configuration without installation

## Web Interface

Try the **[Ds Battery Spacer Generator](https://generate.dssoft.ch)** online - no installation required!

The web interface provides an intuitive GUI where you can:
- Adjust all parameters with real-time preview
- Toggle between linear and slanted layouts
- Download the generated OpenSCAD file directly

Simply open `index.html` in your browser or host it on any web server.

## Local Installation (C++)

### Prerequisites

**Linux:**
```bash
sudo apt install g++ build-essential  # Debian/Ubuntu
sudo dnf install g++                  # Fedora (Tested by Daniel)
sudo pacman -S gcc                    # Arch
```

**Windows:**
Install [Cygwin](https://www.cygwin.com/) or [MinGW-w64](https://www.mingw-w64.org/) with g++ support

**OpenSCAD (for viewing/rendering):**
- Download from [openscad.org](https://openscad.org/downloads.html)
- Ensure OpenSCAD is in your system PATH

### Compilation

Two versions are available:

**Stable Version** (recommended):
```bash
g++ -std=c++17 battgen.cpp -o spacer
```

**Experimental Version** (with latest features):
```bash
g++ -std=c++17 btest.cpp -o spacer
```

### Usage

1. **Run the generator:**
```bash
   ./spacer
```

2. **View/render in OpenSCAD:**
```bash
   openscad battery_spacer.scad
```

3. **Export for 3D printing:**
   - In OpenSCAD: `Design` → `Render` (F6)
   - Export: `File` → `Export` → `Export as STL`

## Configuration Parameters

Edit the `USER PARAMETERS` section in the C++ scripts or use the web interface:

| Parameter | Description | Default | Unit |
|-----------|-------------|---------|------|
| `series_cells` | Number of cells in series (e.g., 6S) | 6 | count |
| `parallel_cells` | Number of parallel rows | 5 | count |
| `slanted` | Enable hexagonal (slanted) layout | true | bool |
| `cell_d` | Cell diameter (18650 = 18.3mm) | 18.3 | mm |
| `adjust` | Clearance adjustment | 0.2 | mm |
| `wall` | Wall thickness | 5.0 | mm |
| `height` | Spacer height | 12.0 | mm |
| `corner_r` | Corner radius | 8.0 | mm |
| `wire_d` | BMS wire hole diameter | 3.0 | mm |
| `ring_h` | Insulation ring height | 0.4 | mm |

## Layout Types

### Linear Layout
```
○ ○ ○ ○ ○ ○
○ ○ ○ ○ ○ ○
○ ○ ○ ○ ○ ○
```
- Simple, traditional arrangement
- Easier to assemble
- More wire routing space

### Slanted Layout (Hexagonal)
```
○ ○ ○ ○ ○ ○
 ○ ○ ○ ○ ○ ○
○ ○ ○ ○ ○ ○
```
- ~15% more compact
- Better structural rigidity
- Optimal space utilization

## File Structure
```
battery-spacer-generator/
├── dev\battgen.cpp          # Stable C++ generator
├── dev\btest.cpp            # Experimental C++ generator
├── index.html           # Web interface (standalone)
├── README.md            # This file
└── LICENSE  # License
```

## Troubleshooting

**Issue: "openscad: command not found"**
- Ensure OpenSCAD is installed and in your PATH
- Linux: `export PATH=$PATH:/path/to/openscad`
- Windows: Add OpenSCAD directory to system PATH

**Issue: Holes don't line up with battery pack**
- Verify `series_cells` matches your pack configuration
- Check if you need `slanted = true` or `false`
- Adjust `adjust` parameter for tighter/looser fit

**Issue: Wire holes missing or incorrect count**
- Linear layout: Should have `series_cells + 1` holes per side
- Slanted layout: Should have `series_cells` holes per side
- Ensure you're using the latest stable version

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Acknowledgments

Designed for makers, hobbyists, and DIY battery pack builders who need custom spacers for their projects.

---

**Happy Building! 🔋⚡**
