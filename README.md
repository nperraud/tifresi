# tifresi: Time Frequency Spectrogram Inversion
'tifresi' to be pronounced 'tifreeezy' provide a simple implementation of TF and spectrogam suitable for inversion, i.e. with a high quality phase recovery.
The phase recovery algorithm used is PGHI (phase gradient heap integration).

## Installation

This repository use the ltfatpy packages that requires a few libraries to be installed. The package relies on some libraries that have to be installed beforehands. To avoid any issue, please perform these steps in order and use a virtual environnement (pipenv, virtualenv or conda).

1. Install `fftw3`, `lapack` and `cmake`
   * On debian based unix system:
    ```
    sudo apt-get install libfftw3-dev liblapack-dev cmake
    ```
   * On MacOS X using homebrew:
    ```
    brew install fftw lapack cmake
    ```
   * On MacOS X using port:
    ```
    sudo port install fftw-3 fftw-3-single lapack cmake
    ```
2. Install cython (required for installing ltfatpy):
    ```
    pip install cython
    ```
3. Install the package from source
    ```
    git clone git@github.com:nperraud/tifresi.git
    cd tifresi
    pip install .
    ```       


## Installation using conda (tested on an M1 mac)

1. Create a conda environment
    ```
    conda create -n tifresi python=3.10
    conda activate tifresi
    ```
2. Install `fftw3`, `lapack` and `cmake`
    ```
    conda install -c conda-forge fftw lapack cmake
    ```
    If it cannot resolve the environnement, you can try:
    ```
    conda install fftw lapack cmake
    ```
3. Install cython (required for installing ltfatpy):
    ```
    conda install cython
    ```

4. Install the package from source
    ```
    git clone git@github.com:nperraud/tifresi.git
    cd tifresi
    pip install -e ".[dev]"
    ```
    Here, the `-e` option is important as it allows to modify the source code without reinstalling the package.
    The `.[dev]` option install the package with the dependencies required for development (pytest, flake8, etc.).

Troubleshooting:
  * Pip fails, update pip
    ```
    python -m pip install --upgrade pip
    ```
  * If `librhash.dylib` or `librhash.0.dylib` is missing, install `rhash`
    ```
    conda install -c anaconda rhash
    ```
    On a mac, you may need to create a symlink
    ```
    ln -s ~/miniconda3/envs/tifresi/lib/librhash.dylib ~/miniconda3/envs/tifresi/lib/librhash.0.dylib
    ```
    Here `tifresi` is the name of the conda environment.
  * If you have error like ` Cannot convert 'const double complex *' to Python object`, you may need to recompile the cython files:
    ```
    find ltfatpy/comp -name '*.pyx' -exec cython -2 {} \;
    ```
    The problem is likely that `-2` argument is missing in the automatic configuration. I do not know how to fix this issue yet.


## Starting
After installation of the requirements, you can check the following notebooks:
* `demo.ipynb` illustrates how to construct a spectrogram and invert it.
* `demo-mel.ipynb` illustrates how to compute a mel spectrogram with the setting used in this repository.


## License & citation

The content of this repository is released under the terms of the [MIT license](LICENCE.txt).
Please consider citing our papers if you use it.

```
@inproceedings{marafioti2019adversarial,
  title={Adversarial Generation of Time-Frequency Features with application in audio synthesis},
  author={Marafioti, Andr{\'e}s and Perraudin, Nathana{\"e}l and Holighaus, Nicki and Majdak, Piotr},
  booktitle={International Conference on Machine Learning},
  pages={4352--4362},
  year={2019}
}
```

```
@article{pruuvsa2017noniterative,
  title={A noniterative method for reconstruction of phase from STFT magnitude},
  author={Pr{\uu}{\v{s}}a, Zden{\v{e}}k and Balazs, Peter and S{\o}ndergaard, Peter Lempel},
  journal={IEEE/ACM Transactions on Audio, Speech, and Language Processing},
  volume={25},
  number={5},
  pages={1154--1164},
  year={2017},
  publisher={IEEE}
}
```

## Developing
As a developer, you can test the package using `pytest`:
```
pip install pytest
```
Then run tests using
```
pytest tifresi
```
You can also use the source code checker `flake8`:
```
pip install flake8
```
Then run tests using
```
flake8 .
```


#### TODO
* Improve doc
* Put the documentation on readthedoc or somthing similar



