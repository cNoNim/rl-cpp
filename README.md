<a name="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

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
      <a href="#about">About</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#building">Building</a></li>
      </ul>
    </li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About

### Built With

[![PyTorch][PyTorch.js]][PyTorch-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

[Miniconda][miniconda-url]

### Building

1. Install dependencies
   ```sh
   conda env update --file environment.yml --name .conda --prefix .conda
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
[contributors-shield]: https://img.shields.io/github/contributors/cNoNim/rl-cpp.svg?style=for-the-badge
[contributors-url]: https://github.com/cNoNim/rl-cpp/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/cNoNim/rl-cpp.svg?style=for-the-badge
[forks-url]: https://github.com/cNoNim/rl-cpp/network/members
[stars-shield]: https://img.shields.io/github/stars/cNoNim/rl-cpp.svg?style=for-the-badge
[stars-url]: https://github.com/cNoNim/rl-cpp/stargazers
[issues-shield]: https://img.shields.io/github/issues/cNoNim/rl-cpp.svg?style=for-the-badge
[issues-url]: https://github.com/cNoNim/rl-cpp/issues
[license-shield]: https://img.shields.io/github/license/cNoNim/rl-cpp.svg?style=for-the-badge
[license-url]: https://github.com/cNoNim/rl-cpp/blob/master/LICENSE
[pytorch.js]: https://img.shields.io/badge/pytorch-000000?style=for-the-badge&logo=pytorch
[pytorch-url]: https://pytorch.org/
[miniconda-url]: https://conda.io/miniconda.html
