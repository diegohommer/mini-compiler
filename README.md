<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![project_license][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/diegohommer/mini-compiler">
  </a>

<h3 align="center">mini-compiler</h3>

  <p align="center">
    project_description
    <br />
    <a href="https://github.com/diegohommer/mini-compiler"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/diegohommer/mini-compiler">View Demo</a>
    &middot;
    <a href="https://github.com/diegohommer/mini-compiler/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/diegohommer/mini-compiler/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<!-- [![Product Name Screen Shot][product-screenshot]](https://example.com) -->
This `mini-compiler` project is an educational endeavor to build a functional compiler for a simplified programming language. It meticulously implements the traditional stages of compilation, demonstrating the process of transforming source code through an Intermediate Language (ILOC) into executable x86 assembly code. The project primarily focuses on understanding and implementing the core principles of lexical analysis, parsing, semantic analysis, intermediate code generation, and target code generation.

**Key Features:**

- **Program Structure:** Supports functions, global variables, and sequential execution.
- **Data Types:** `int` and `float` for variables and literals.
- **Control Flow:** `if-else` and `while` statements.
- **Variables:** Declaration, initialization, and assignment.
- **Function Calls:** User-defined functions with arguments.
- **Expressions:** Arithmetic, comparison, bitwise, and unary operations.
- **Return Statements:** Type-checked return values.
- **Scoping:** Block and function-level scope management.
- **Frontend Integration:** Lexical, syntactic, and semantic analysis.
- **ILOC Generation:** Outputs machine-independent intermediate code.
- **x86 Assembly:** Translates ILOC to x86 assembly.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With
* [![C][C-shield]][C-url]
* [![Flex][Flex-shield]][Flex-url]
* [![Bison][Bison-shield]][Bison-url]
* [![Python][Python-shield]][Python-url]
* [![GNU Make][Make-shield]][Make-url]
<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

Follow the steps below to set up and build the project locally.

### Prerequisites

Make sure the following tools are installed on your system:

* **Flex**
  ```sh
  sudo apt install flex
  ```
* **Bison**
  ```sh
  sudo apt install bison
  ```
* **GCC**
  ```sh
  sudo apt install build-essential
  ```
* **Make**
  ```sh
  sudo apt install make
  ```

### Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/diegohommer/mini-compiler.git
   cd mini-compiler
   ```
2. Build the project using make:
   ```sh
   make
   ```
3. Change git remote url to avoid accidental pushes to base project
   ```sh
   git remote set-url origin YOUR_USERNAME/mini-compiler
   git remote -v  # Confirm the changes
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [ ] Feature 1
- [ ] Feature 2
- [ ] Feature 3
    - [ ] Nested Feature

See the [open issues](https://github.com/diegohommer/mini-compiler/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Top contributors:

<a href="https://github.com/diegohommer/mini-compiler/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=diegohommer/mini-compiler" alt="contrib.rocks image" />
</a>



<!-- LICENSE -->
## License

Distributed under the project_license. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Your Name - [@twitter_handle](https://twitter.com/twitter_handle) - email@email_client.com

Project Link: [https://github.com/diegohommer/mini-compiler](https://github.com/diegohommer/mini-compiler)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* []()
* []()
* []()

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/diegohommer/mini-compiler.svg?style=for-the-badge
[contributors-url]: https://github.com/diegohommer/mini-compiler/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/diegohommer/mini-compiler.svg?style=for-the-badge
[forks-url]: https://github.com/diegohommer/mini-compiler/network/members
[stars-shield]: https://img.shields.io/github/stars/diegohommer/mini-compiler.svg?style=for-the-badge
[stars-url]: https://github.com/diegohommer/mini-compiler/stargazers
[issues-shield]: https://img.shields.io/github/issues/diegohommer/mini-compiler.svg?style=for-the-badge
[issues-url]: https://github.com/diegohommer/mini-compiler/issues
[license-shield]: https://img.shields.io/github/license/diegohommer/mini-compiler.svg?style=for-the-badge
[license-url]: https://github.com/diegohommer/mini-compiler/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/diegohommer
[C-shield]: https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
[C-url]: https://en.wikipedia.org/wiki/C_(programming_language)
[Flex-shield]: https://img.shields.io/badge/Flex-orange?style=for-the-badge&logoColor=white
[Flex-url]: https://github.com/westes/flex
[Bison-shield]: https://img.shields.io/badge/Bison-8B0000?style=for-the-badge&logoColor=white
[Bison-url]: https://www.gnu.org/software/bison/
[Python-shield]: https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white
[Python-url]: https://www.python.org/
[Make-shield]: https://img.shields.io/badge/GNU%20Make-467008?style=for-the-badge&logo=gnuemacs&logoColor=white
[Make-url]: https://www.gnu.org/software/make/
