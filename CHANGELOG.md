# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added

- GitHub showcase tree: `firmware/BT9/`, `app/B4-tracker.aia`, bilingual README (zh-Hant / en), hardware / protocol / firmware / survey notes.
- MIT licence, contributing guide, and a curated set of de-identified hardware photographs plus three official survey charts.
- Hero artwork and documentation index under `docs/`.

### Changed

- Removed the first duplicate `volume()` in `BT9.ino` so the sketch compiles. Playback behaviour matches the 2021 file.
- Product `app/B4-tracker.aia` no longer contains the classroom mini-game (Screen3 / 猶戲). The unaltered 2021 AIA is in `local/minigame/` (gitignored).

### Removed

- Working dump (`original-data/`, ~8.6 GB) is gitignored: films, vendor trees, raw questionnaire rows, draft sketches, and copyrighted BGM files stay on disk only.
- Mini-game assets (`pop.m4a`, POPMenway art) and the unused leftover bitmaps are out of the public AIA.

## [1.0.0] - 2021-12-28

### Added

- Capstone hand-in for class 3-Geng, stream B, group 4 (academic year ROC 110).
- ESP32 firmware BT9 (TUNIOT + hand edits): SPP commands, five classroom tones, volume `vi0`–`vi6`, lamp, on-device stop.
- MIT App Inventor project `esp_teat10-0914-2` (imported here as `app/B4-tracker.aia`).
- Concept film (now at https://youtu.be/bfgeC1RDvt0) and a public questionnaire (370 sent, 354 valid, 25–28 December 2021).
