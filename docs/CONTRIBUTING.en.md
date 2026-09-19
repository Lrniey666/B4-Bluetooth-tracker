# Contributing

Languages: [繁體中文](../CONTRIBUTING.md) · [English](CONTRIBUTING.en.md)

This repository is an archived 2021 school capstone. Documentation fixes, build notes and **original** prompt tones are welcome. Treat the firmware as an artefact first.

## Before you start

1. Read [`../README.md`](../README.md) and [`README.md`](README.md).
2. Pin or command changes must land in [`hardware.md`](hardware.md), [`protocol.md`](protocol.md) and `firmware/BT9/BT9.ino` together.
3. When they disagree: **2021 submitted behaviour wins**. Fix the docs; do not modernise BT9 and call it the original.

## Conventions

| Item | Rule |
| --- | --- |
| Public copy | Traditional Chinese in `README.md`; English in `docs/README.en.md` — edit both |
| Dates | `YYYY-MM-DD`, Taipei |
| Changelog | `## [Unreleased]` in `CHANGELOG.md` (Keep a Changelog 2.0.0) |
| Line endings | LF (`.gitattributes`) |

## Please do not

- Commit `original-data/`, `local/`, films, `.pds` files, survey csv/xlsx, or free-text replies
- Commit photographs that identify teammates or respondents
- Add full scores or BGM files of well-known tunes
- Hard-code absolute machine paths or accounts
- Refactor the TUNIOT output for taste and present it as the 2021 hand-in

Ask first before irreversible git history changes.

## After a change

1. Note it under `## [Unreleased]` in `CHANGELOG.md`
2. Keep the two README tours in step if you touch the hero, install steps or tree
3. If `BT9.ino` changes, record the drift from the submitted file in [`firmware.md`](firmware.md)
