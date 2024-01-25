<a name="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![Build][build-shield]][build-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">Reinforcement Learning C++</h3>

  <p align="center">
    ·
    <a href="https://github.com/cNoNim/rl-cpp/issues">Report Bug</a>
    ·
    <a href="https://github.com/cNoNim/rl-cpp/issues">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#building">Building</a></li>
      </ul>
    </li>
  </ol>
</details>

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

[Miniconda][miniconda-url]

[NVidia Cuda Toolkit][cuda-url]

### Building

1. Install dependencies
   ```sh
   conda env update --file environment.yml --prefix .conda
   ```
2. Configure
   ```sh
   cmake --preset default -B build
   ```
4. Build
   ```sh
   cmake --build build
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/cNoNim/rl-cpp.svg?style=flat
[contributors-url]: https://github.com/cNoNim/rl-cpp/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/cNoNim/rl-cpp.svg?style=flat
[forks-url]: https://github.com/cNoNim/rl-cpp/network/members
[stars-shield]: https://img.shields.io/github/stars/cNoNim/rl-cpp.svg?style=flat
[stars-url]: https://github.com/cNoNim/rl-cpp/stargazers
[issues-shield]: https://img.shields.io/github/issues/cNoNim/rl-cpp.svg?style=flat
[issues-url]: https://github.com/cNoNim/rl-cpp/issues
[license-shield]: https://img.shields.io/github/license/cNoNim/rl-cpp.svg?style=flat
[license-url]: https://github.com/cNoNim/rl-cpp/blob/master/LICENSE
[build-shield]: https://github.com/cNoNim/rl-cpp/actions/workflows/build.yml/badge.svg
[build-url]: https://github.com/cNoNim/rl-cpp/actions/workflows/build.yml
[pytorch-url]: https://pytorch.org/
[cuda-url]: https://developer.nvidia.com/cuda-toolkit
[miniconda-url]: https://conda.io/miniconda.html
