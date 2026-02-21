# Paint Premium

A paint application built with C++ and SFML.

## Features

- **Drawing**: Left-click and drag to draw with a circular brush
- **Eraser**: Right-click and drag to erase drawn content
- **Adjustable Brush Size**: Use mouse scroll wheel to increase/decrease brush size
- **Custom Colors**: Set brush color using RGB values (0-255)
- **Save Canvas**: Export your artwork as an image file
- **Clear Canvas**: Erase all content at once
- **Undo**: Press `Ctrl+Z` to undo the last stroke

## Controls

| Action | Control |
|--------|---------|
| Draw | Left-click + drag |
| Erase | Right-click + drag |
| Increase brush size | Scroll wheel up |
| Decrease brush size | Scroll wheel down |
| Undo last stroke | `Ctrl + Z` |

## UI Elements

- **Save Button**: Opens a file dialog to save the canvas as an image
- **R/G/B Text Fields**: Input RGB color values (0-255)
- **Save Color Button**: Apply the RGB values to the brush color
- **Erase Button**: Clear the entire canvas

## Requirements

- g++
- SFML
- Windows (It only runs on Windows)

## Project Structure

```
Paint-Premium/
├── headers/
│   ├── app.hpp
│   ├── button.hpp
│   ├── textzone.hpp
│   └── tools.hpp
└── src/
    ├── main.cpp
    ├── app.cpp
    ├── button.cpp
    └── textzone.cpp
```