#include "EnergyHistogram.hh"

#include "G4SystemOfUnits.hh"
using CLHEP::keV;

EnergyHistogram::EnergyHistogram(const int nBins, const double Emin, const double Emax) : m_nBins(nBins), m_Emin(Emin), m_Emax(Emax)
{
    h1 = new TH1D( "h1","h1", m_nBins, m_Emin, m_Emax );
    t1 = new TTree( "t1", "t1" );
    t1->Branch("Energy",&Energy,"Energy/D");
//    m_histogram = new double[m_nBins+2];
    Reset();
}

EnergyHistogram::~EnergyHistogram()
{
    delete h1;
//    delete[] m_histogram;
}


void EnergyHistogram::Reset()
{
    G4AutoLock lock();
//    m_histogram = new double[m_nBins+2];
//    for (int i = 0; i <= m_nBins+1; i++)
//    {
//        m_histogram[i] = 0;
//    }

}

void EnergyHistogram::Fill(const double energy)
{
    G4AutoLock lock();
    Energy = energy;
    t1->Fill( );
    if (energy < m_Emin)
    {
        h1->Fill( 0 );
    //    m_histogram[0] += 1;
    }
    else if (energy > m_Emax)
    {
        h1->Fill( 0 );
    //    m_histogram[m_nBins+1] += 1;
    }
    else
    {
        h1->Fill( energy );
    //    m_histogram[int(m_nBins*(energy-m_Emin)/(m_Emax-m_Emin)) + 1] += 1;
    }
}

void EnergyHistogram::Write(const string fileName) const
{
//    ofstream fout(fileName);
//    fout << m_nBins << "\t" << m_Emin/keV << "\t" << m_Emax/keV << "\n";
//    for (int i = 0; i <= m_nBins+1; i++)
//    {
//        fout << m_histogram[i] << "\n";
//    }

    TFile* f1 = new TFile( fileName.c_str( ), "RECREATE" );

    h1->Write( );
    t1->Write( );

    f1->Close( );
}
