#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH2D.h"
#include "TH1D.h"




int main( int argc, char* argv[] ) {

  if( argc==1 ) {
    std::cout << "USAGE: ./signal-noise_comparison [fileName]" << std::endl;
    exit(1);
  }

  std::string fileName(argv[1]);

  std::cout << "-> Opening file: " << fileName << std::endl;

  TString fileName_tstr(fileName);


  TFile *output_f=new TFile("signal-noise_comparison.root","RECREATE");

  TFile* file = TFile::Open( fileName.c_str() );
  TTree* tree = (TTree*)file->Get("tree");

  TFile* file_ref = TFile::Open("../../noise/analysis/Run_APDWL_HV405_noise_K_Measurements_Only_1_12_2023.root");
  TTree* tree_ref = (TTree*)file_ref->Get("tree");


  float vamp; 
  float vamp_ref;
  tree->SetBranchAddress( "vamp", &vamp );
  tree_ref->SetBranchAddress( "vamp", &vamp_ref );

  std::cout<<"branch vamp setted"<<std::endl;

  TH1D* h1 = new TH1D( "vamp", "", 500, -0.1, 0.1 );

  int nentries = tree->GetEntries();

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry(iEntry);
    h1->Fill( vamp );

  }
  std::cout<<"first histo filled"<<std::endl;

  TCanvas* c1 = new TCanvas( "c1", "", 600., 600. );
  c1->cd();
 // Double_t scale = h1->GetXaxis()->GetBinWidth(1)/(h1->Integral());
 // h1->Scale(scale);      //metodo3
 // h1->Scale(1./nentries);     //metodo2
 // h1->Scale(1./h1->Integral());  //metodo1
  h1->Draw("HISTO");
 // h1->DrawNormalized("HISTO");    //metodo6

  TH1D* h2 = new TH1D( "vamp_ref", "", 100, -0.04, 0.04);

  int nentries_ref = tree_ref->GetEntries();

  for( unsigned iEntry=0; iEntry<nentries_ref; ++iEntry ) {

    tree_ref->GetEntry(iEntry);
    h2->Fill( vamp_ref );

  }
  std::cout<<"second histo filled"<<std::endl;
  c1->cd();

  int e=0;
  for(unsigned bin=0; bin<100; ++bin){ 
  Double_t binContent = h2->GetBinContent(bin);
  if (binContent == 0) e++;
  }
  std::cout<<"empty bins:"<<e<<std::endl;

 // Double_t scale2 = h2->GetXaxis()->GetBinWidth(1)/(h2->Integral());
 // h2->Scale(scale2);         //metodo3
 // h2->Scale(100, "nosw2");      //metodo4
 // h2->Scale(1./h2->Integral());   //metodo1
 // h2->Scale(1./nentries_ref);    //metodo2
  h2->Scale(h1->Integral()/h2->Integral()); //metodo5
  h2->SetLineColor(2);
  h2->Draw("HISTO SAME"); 
 //h2->DrawNormalized( "HISTO SAME");   //metodo6

  output_f->cd();
  c1->Write();
  output_f->Close();

  std::cout << "-> Tree saved in: " << output_f->GetName() << std::endl;

  return 0;
} 

