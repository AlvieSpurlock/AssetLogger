# Asset Logger

Asset Logger is a lightweight console tool built for indie game developers who need a fast, reliable way to document game assets for Steam submissions. It collects platform, usage, creator, and license information, then exports everything into a clean, submission‑ready CSV file.

## Overview

Steam requires developers to provide clear documentation of every third‑party asset used in their game. Asset Logger streamlines this process by guiding you through each required field, validating your inputs, and generating a structured CSV that can be dropped directly into your submission package.

This makes Asset Logger ideal for:

- Indie developers preparing Steam submissions

- Teams tracking asset usage across levels or scenes

- Solo devs who want clean documentation without spreadsheets

- Anyone who needs a simple, repeatable asset‑logging workflow

The tool runs entirely in the console, keeping the process quick, focused, and distraction‑free.

## Why Asset Logger?

Manually tracking assets in spreadsheets is slow, error‑prone, and easy to forget. Asset Logger solves this by:

- Prompting for every required field

- Ensuring consistent formatting

- Preventing duplicate or missing entries

- Confirming each asset before saving

- Exporting everything into a clean CSV

- Supporting multiple assets in a single session

Instead of juggling documents or re‑typing the same fields, you get a guided workflow that guarantees accuracy.

## Core Features

### Guided Asset Entry

Asset Logger walks you through each required field:

- Platform (e.g., Fab, Unity Store, Unreal Marketplace)

- Location in game

- Usage in game

- Asset name

- Asset creator

- License type

### Confirmation Step

After entering an asset, the tool displays a formatted summary and asks:

```txt
Is this correct?
1: Yes
2: No
```

This prevents incorrect or incomplete entries from being saved.

### Multi‑Asset Support

After each entry, you can choose:

```txt
Do you have more assets?
1: Yes
2: No
```

Asset Logger continues collecting until you're done.

### CSV Export

All assets are exported into a clean, comma‑separated file with the following structure:

- Platform,Location,Use,Asset Name,Asset Maker,License Type

Perfect for Steam’s documentation requirements.

### File Safety

If the CSV already exists, Asset Logger will warn you and ask whether to overwrite it, preventing accidental data loss.

## Example Workflow

```txt
Platform: Fab
Asset Location (in Game): Main Level
Asset Use (in Game): Rocks
Asset Name (on Platform): Modular Rock Pack
Asset Maker (on Platform): Rocker
License Type: Professional

Is this correct?
1: Yes
2: No

Do you have more assets?
1: Yes
2: No

If the file already exists:

File Already Exists
Do you want to overwrite it?
1: Yes
2: No
```

### Output Example

```txt
------Results------
Platform,Location(in Game),Use(in Game),Asset Name(on Platform),Asset Maker(on Platform),License Type
1: Fab,Main Level,Rocks,Modular Rock Pack,Rocker,Professional
```

## Summary

Asset Logger is a simple, fast, and reliable tool for documenting game assets during Steam submissions. By guiding developers through each required field and exporting everything into a clean CSV, it removes the stress and guesswork from asset compliance.

Requires: Python 3.x
