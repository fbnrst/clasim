from . import cclasim as _clasim


def run(mode=2, samples=1000, nCells=100, mCells=100, times=[0.0,0.2], GF=1,
        G1=0.2, S=0.3, G2M=0.5, sCells=0.01, sSamples=0.01, seed=42):
    """Run a simulation of a continuous labeling assay
    
    Parameters
    ----------
    mode : int
      either 1 for 1 daughter cell or 2 for 2 daughter cells
    samples : int
        number of samples per timepoint
    nCells : int
        number of cells per samples
    mCells : int
        number of measured cells (only for `mode=2`)
    times : List of float
        list of timepoints
    GF : float
        growth fraction
    G1 : float
        length of G1-phase
    S : float
        length of S-phase
    G2M : float
        length of G2- and M-phase
        cell cycle Tc = G1 + S + G2M
    sSamples : float
        standard deviation for sample level variability
    sCells : float
        standard deviation for cell level variability
    seed : int
        Random seed
    
    Returns
    -------
    res : list
        A list containing numbers of labelled cells.
    """
    return _clasim.run(seed, samples, nCells, mCells, times, GF, G1, S, G2M, 
                       sCells, sSamples, mode)
