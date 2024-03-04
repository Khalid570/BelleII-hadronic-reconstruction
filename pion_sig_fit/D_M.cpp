void D_M()
{
  using namespace RooFit;
  gROOT->LoadMacro("~/Belle2Style.C");

  TCanvas *canvas1 = new TCanvas("canvas1", "canvas1", 600,800);
  canvas1->Draw();
  canvas1->cd();

  TFile *input = new TFile("~/Root_files/Local_Signal_MC/Bu2DK_pi.root");
  TTree *tree = (TTree*)input->Get("tree");
  //TLatex Tl;

  RooRealVar D_M("D_M","D_M",1.8,1.9);
  RooDataSet data("data","dataset", tree, RooArgList(D_M));

  //signal
  RooRealVar mean("mean","mean",1.868996, 1.8, 1.9);
  RooRealVar sigma("sigma", "sigma", 0.014881, 0.0001, 1);
  RooGaussian sig("sig","signal p.d.f.", D_M, mean, sigma);
  
  //bkg
  RooRealVar c0("c0", "coefficient #0",  0.624903, -1.0, 1.0);
  RooRealVar c1("c1", "coefficient #1", -0.570249, -1.0, 1.0);
  RooRealVar c2("c2", "coefficient #2", -0.58301, -1.0, 1.0);
  RooRealVar c3("c3", "coefficient #3", 0.1, -1.0, 1.0);
  RooRealVar c4("c4", "coefficient #4", -0.1, -1.0, 1.0);
  RooChebychev bkg("bkg", "background p.d.f.", D_M, RooArgList(c0, c1, c2, c3, c4));
  RooRealVar fsig("fsig","signal fraction", 0.5, 0., 1.);

  //model
  RooAddPdf model("model","model", RooArgList(sig,bkg),fsig);
  model.fitTo(data);
    
    
  canvas1->cd();
  TPad *pad1 = new TPad("pad1","data pad",0.1,0.5,1.,1.);
  pad1->Draw();
  pad1->cd();
  RooPlot *frame = D_M.frame();
  data.plotOn(frame);
  model.plotOn(frame, Components(sig), LineColor(kRed), LineStyle(kDashed));
  model.plotOn(frame, Components(bkg), LineColor(kGreen), LineStyle(kDashed));
  model.plotOn(frame);
  frame->SetTitle("#bar{B^{0}} #rightarrow D^{+} (K^{-} #pi^{+} #pi^{+}) #pi^{-} ");
  frame->GetYaxis()->SetTitle("Events");
  frame->GetXaxis()->SetTitle("D InvM [GeV/c^{2}]");
  gPad->SetLeftMargin(0.2);
  frame->Draw();
  canvas1->Update();


  canvas1->cd();
  TPad *pad2 = new TPad("pad2","pull pad",0.1,0.3,1.,0.5);
  pad2->Draw();
  pad2->cd();
  RooHist* hpull = frame->pullHist();
  RooPlot* frame2 = D_M.frame();
  frame2->addPlotable(hpull,"P");
  frame2->SetTitle("");
  frame2->GetYaxis()->SetTitle("Pull");
  frame2->GetXaxis()->SetTitleSize(0.1);
  frame2->GetXaxis()->SetLabelSize(0.1);
  frame2->GetYaxis()->SetTitleSize(0.1);
  frame2->GetYaxis()->SetLabelSize(0.1);
  frame2->GetYaxis()->SetTitleOffset(0.5);
  frame2->GetYaxis()->SetNdivisions(602);
  frame2->SetAxisRange(-6., 6.,"Y");
  frame2->GetXaxis()->SetTitle("D InvM [GeV/c^{2}]");
  gPad->SetLeftMargin(0.2);
  gPad->SetBottomMargin(0.1);
  frame2->Draw();

  TLine *line = new TLine(1.8,0,1.9,0);
  line->SetLineColor(kBlack);
  line->Draw();
  
  canvas1->cd();
  TPad *pad3 = new TPad("pad3","residual pad",0.1,0.1,1.,0.3);
  pad3->Draw();
  pad3->cd();
  RooHist* hresid = frame->residHist() ;
  RooPlot* frame3 = D_M.frame();
  frame3->addPlotable(hresid,"P");
  frame3->SetTitle("");
  frame3->GetYaxis()->SetTitle("Residual");
  frame3->GetXaxis()->SetTitleSize(0.1);
  frame3->GetXaxis()->SetLabelSize(0.1);
  frame3->GetYaxis()->SetTitleSize(0.1);
  frame3->GetYaxis()->SetLabelSize(0.1);
  frame3->GetYaxis()->SetTitleOffset(0.5);
  frame3->GetYaxis()->SetNdivisions(10);
  frame3->SetAxisRange(-250., 250.,"Y");
  frame3->GetXaxis()->SetTitle("D InvM [GeV/c^{2}]");
  gPad->SetLeftMargin(0.2);
  gPad->SetBottomMargin(0.1);
  frame3->Draw();

  TLine *line1 = new TLine(1.8,0,1.9,0);
  line->SetLineColor(kBlack);
  line->Draw();
  
  
  cout << "chi^2/ndf = " << frame->chiSquare() << endl;
  data.Print();
  Double_t low_cut = mean.getVal() - 3*sigma.getVal();
  Double_t high_cut = mean.getVal() + 3*sigma.getVal();
  cout << " sigma =" << sigma.getValV() << endl << endl;
  cout << "low cut =" << low_cut << endl;
  cout << "highcut =" << high_cut << endl;



  pad1->cd();
  TLatex* mean_txt = new TLatex();
  mean_txt->DrawLatex(1.81, 2500, "#scale[0.8]{#color[6]{mean = 1.869 GeV/C^{2}}}");
  mean_txt->Draw();
  TLatex* sigma_txt = new TLatex();
  sigma_txt->DrawLatex(1.81, 2100, "#scale[0.8]{#color[6]{#sigma = 0.003775 GeV/C^{2}}}");
  sigma_txt->Draw();
  TLatex* lower_txt = new TLatex();
  lower_txt->DrawLatex(1.81, 1700, "#scale[0.8]{#color[6]{lower cut = 1.858 GeV/C^{2}}}");
  lower_txt->Draw();
  TLatex* upper_txt = new TLatex();
  upper_txt->DrawLatex(1.81, 1300, "#scale[0.8]{#color[6]{upper cut = 1.881 GeV/C^{2}}}");
  upper_txt->Draw();
  TLatex* chi_txt = new TLatex();
  chi_txt->DrawLatex(1.81, 900, "#scale[0.8]{#color[6]{#chi^{2} = 4.147}}");
  chi_txt->Draw();

  canvas1->cd();

  pad1->cd();
  TArrow *arrow1 = new TArrow(low_cut,125,low_cut,75,0.01);
  arrow1->SetLineWidth(2);
  arrow1->SetLineColor(kRed);
  arrow1->Draw();
  TArrow *arrow2 = new TArrow(high_cut,125,high_cut,75,0.01);
  arrow2->SetLineWidth(2);
  arrow2->SetLineColor(kRed);
  arrow2->Draw();
  canvas1->cd();
    
  canvas1->SaveAs("D_M_fit.pdf");
}
