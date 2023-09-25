#include "Construction.hh"        // 헤더파일 참조 

MyDetectorConstruction::MyDetectorConstruction()     // 생성자
{}

MyDetectorConstruction::~MyDetectorConstruction()    // 소멸자 
{}

G4VPhysicalVolume* MyDetectorConstruction::Construct()   
{ 

  /////materials       
  // Mat = material의 준말 

  G4NistManager * nist = G4NistManager::Instance();
  G4Material* Mat_Air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* Mat_Water = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* Mat_Teflon = nist->FindOrBuildMaterial("G4_TEFLON");
    

  G4bool checkoverlaps = true ;

  /// 기본적으로 대부분의 작명 방식을 통일함 (헷갈리지 않기 위함)
  /// solid_Abc, logical_Abc, physical_Abc    -> 물체 첫글자는 대문자
  /// "solid_abc", "logical_abc", "physical_abc" -> 물체 첫글자도 소문자
  /// 물체의 이름에 정보가 다 포함되어 있을수록 나중에 헷갈릴 확률이 적음 

  /////world

  G4double world_size = 10 * m ;

  G4Box * solid_World = new G4Box("solid_world",
                                  world_size,
                                  world_size,
                                  world_size);
                          
  G4LogicalVolume * logical_World = new G4LogicalVolume(solid_World,
                                                        Mat_Air,
                                                        "logical_world");

  G4VPhysicalVolume * physical_World = new G4PVPlacement(0,
                                                          G4ThreeVector(),
                                                          logical_World,
                                                          "physical_world",
                                                          0,
                                                          false,
                                                          0,
                                                          checkoverlaps);

  /////teflon tub

  G4double teflon_innerRadius = 0. ;
  G4double teflon_outerRadius = 0.6 * m ;
  G4double teflon_height = 7. * m ;

  G4Tubs * solid_Teflon = new G4Tubs("solid_teflon",
                                    teflon_innerRadius,
                                    teflon_outerRadius,
                                    .5 * teflon_height,
                                    0.,
                                    2. * M_PI);

  G4LogicalVolume * logical_Teflon = new G4LogicalVolume(solid_Teflon,
                                                        Mat_Teflon,   
                                                        "logical_teflon");                                   

  G4VPhysicalVolume * physical_Teflon = new G4PVPlacement(0,
                                                          G4ThreeVector(),
                                                          logical_Teflon,
                                                          "physcial_teflon",
                                                          logical_World,
                                                          false,
                                                          0,
                                                          checkoverlaps);

  /////water tub

  G4double tub_innerRadius = 0. ; 
  G4double tub_outerRadius = 0.5 * m ;
  G4double tub_height = 2.5 * m ;

  G4Tubs * solid_tub = new G4Tubs("solid_tub",
                                    tub_innerRadius,
                                    tub_outerRadius,
                                    .5 * tub_height,
                                    0.,
                                    2. * M_PI);

  /////water guide (plus, minus로 왼쪽,오른쪽 solid volume 각각 만든 뒤 unionsolid 로 합침)

  G4double guide_axial = 0.;
  G4double guide_down_R = 0.25 * m ; 
  G4double guide_up_R = 0.5 * m ;
  G4double guide_height = 0.25 * m ;

  G4Cons * solid_Guide_Plus = new G4Cons("solid_guide_plus",
                                        guide_axial,
                                        guide_up_R,
                                        guide_axial, 
                                        guide_down_R,
                                        .5 * guide_height,
                                        0.,
                                        2. * M_PI);

  G4Cons * solid_Guide_Minus = new G4Cons("solid_guide_minus",
                                        guide_axial,
                                        guide_down_R,
                                        guide_axial, 
                                        guide_up_R,
                                        .5 * guide_height,
                                        0.,
                                        2. * M_PI);
  

  ///// Union Water

  // 몸통(튜브) + plus = temp
  // temp + minus = solid_water(최종 solid volume)

  G4UnionSolid * solid_Water_temp = new G4UnionSolid("solid_water_temp",
                                                    solid_tub,
                                                    solid_Guide_Plus,
                                                    0,
                                                    G4ThreeVector(0,0,0.5 * tub_height + 0.5 * guide_height));

  G4UnionSolid * solid_Water = new G4UnionSolid("solid_water",
                                                solid_Water_temp,
                                                solid_Guide_Minus,
                                                0,
                                                G4ThreeVector(0,0,- 0.5 * tub_height - 0.5 * guide_height));                                          

  G4LogicalVolume * logical_Water = new G4LogicalVolume(solid_Water, Mat_Water,"logical_water");
  
  G4VPhysicalVolume * physical_Water = new G4PVPlacement(0,
                                                        G4ThreeVector(),
                                                        logical_Water,
                                                        "physical_water",
                                                        logical_Teflon,
                                                        false,
                                                        0,
                                                        checkoverlaps);

  
  /////PMT
  //두께가 매우 얇은 원기둥으로 생성
  //테플론 속에 위치하며 물과 맞닿게 설정 
  //Pmt라고 쓰면 어색해서 PMT는 다 대문자로 이름 지음

  G4double PMT_height = 0.001 * m ;

  G4Tubs * solid_PMT = new G4Tubs("solid_pmt",
                                  0,
                                  0.25 * m,
                                  0.5 * PMT_height,
                                  0.,
                                  2. * M_PI);
  
  // pmt와 같이 측정 도구로 쓰일 물체는 아래와 같이 설정 
  // 원래는 G4LogicalVolume * logical_PMT = new G4LogicalVolume(solid_PMT,~~~)
  // 같은 방식이지만 PMT의 logical volume을 다른 파일에서도 불러와야하므로 
  // construction.cc파일이 아닌 construction.hh 파일에서 미리 선언
  // 헤더 파일에서 선언한 flogical_PMT에 정보를 넣어주는 방식 


  flogical_PMT = new G4LogicalVolume(solid_PMT,
                                    Mat_Water,
                                    "logical_pmt");

  // 밑에 애들 G4VPhysicalVolume* physical_PMT = new G4~~~ 와 같이 안함
  // 왜그래야하는지는.. 모르겠네.. 일반적인 physical volume처럼 지정하면 에러남. 

  new G4PVPlacement(0,
                    G4ThreeVector(0.,0., - (guide_height + 0.5 * tub_height + 0.5 * PMT_height)),
                    flogical_PMT,
                    "physical_pmt",
                    logical_Water,
                    false,
                    0,                    //똑같은 이름으로 설정했기 때문에 구분해주고자 copynumber 0번과 1번으로 구분.   
                    checkoverlaps);  

  new G4PVPlacement(0,
                    G4ThreeVector(0.,0., (guide_height + 0.5 * tub_height + 0.5 * PMT_height)),
                    flogical_PMT,
                    "physical_pmt",
                    logical_Water,
                    false,
                    1,                   //다른 파일에서 copynumber로 구분하기 때문에 기억해두기   
                    checkoverlaps);  

  /////Surface 

  // teflon 표면: TeflonOpticalSurface 
  // water 표면: WaterOpticalSurface

  G4OpticalSurface * TeflonOpticalSurface = new G4OpticalSurface("TeflonSurface");

  TeflonOpticalSurface->SetType(dielectric_dielectric);  //가장 일반적인 경우 (유전체-유전체)
  TeflonOpticalSurface->SetModel(unified);               //가장 일반적인 모델
  TeflonOpticalSurface->SetFinish(groundfrontpainted);   //난반사 ex: 테플론

  G4OpticalSurface* WaterOpticalSurface = new G4OpticalSurface("WaterSurface");

  WaterOpticalSurface->SetType(dielectric_dielectric);
  WaterOpticalSurface->SetModel(unified);
  WaterOpticalSurface->SetFinish(groundfrontpainted);


  // 광자 입장에서 water->teflon , teflon->water 으로 가는 두 상황 다 설정해줘야함 
  new G4LogicalBorderSurface("teflon_water_LSB", physical_Water, physical_Teflon, TeflonOpticalSurface);
  new G4LogicalBorderSurface("water_teflon_LSB", physical_Teflon, physical_Water, WaterOpticalSurface);

  //teflon, teflon surface 둘 다 물특을 넣어줘야함. 그 전에 물특 변수 지정
  G4MaterialPropertiesTable * TEFLON_MPT = new G4MaterialPropertiesTable();
  G4MaterialPropertiesTable * TeflonInterfaceMpt = new G4MaterialPropertiesTable();

  // 파장 200 nm - 400 nm 에 해당하는 energy
  // 그에 맞게 transmittance, rindex, absorption 설정 
  
  std::vector<G4double> energy = {3.0995*eV, 3.1073*eV, 3.1151*eV, 3.1229*eV, 3.1308*eV, 3.1387*eV, 3.1467*eV, 3.1547*eV, 3.1628*eV, 3.1708*eV,
                                  3.1790*eV, 3.1871*eV, 3.1954*eV, 3.2036*eV, 3.2119*eV, 3.2203*eV, 3.2286*eV, 3.2371*eV, 3.2455*eV, 3.2541*eV, 
                                  3.2626*eV, 3.2712*eV, 3.2799*eV, 3.2886*eV, 3.2973*eV, 3.3061*eV, 3.3150*eV, 3.3239*eV, 3.3328*eV, 3.3418*eV, 
                                  3.3508*eV, 3.3599*eV, 3.3690*eV, 3.3782*eV, 3.3874*eV, 3.3967*eV, 3.4060*eV, 3.4154*eV, 3.4249*eV, 3.4343*eV, 
                                  3.4439*eV, 3.4535*eV, 3.4631*eV, 3.4728*eV, 3.4826*eV, 3.4924*eV, 3.5023*eV, 3.5122*eV, 3.5222*eV, 3.5322*eV, 
                                  3.5423*eV, 3.5524*eV, 3.5626*eV, 3.5729*eV, 3.5832*eV, 3.5936*eV, 3.6041*eV, 3.6146*eV, 3.6251*eV, 3.6358*eV, 
                                  3.6465*eV, 3.6572*eV, 3.6680*eV, 3.6789*eV, 3.6899*eV, 3.7009*eV, 3.7120*eV, 3.7231*eV, 3.7343*eV, 3.7456*eV, 
                                  3.7570*eV, 3.7684*eV, 3.7799*eV, 3.7914*eV, 3.8031*eV, 3.8148*eV, 3.8265*eV, 3.8384*eV, 3.8503*eV, 3.8623*eV, 
                                  3.8744*eV, 3.8865*eV, 3.8987*eV, 3.9110*eV, 3.9234*eV, 3.9359*eV, 3.9484*eV, 3.9610*eV, 3.9737*eV, 3.9865*eV, 
                                  3.9994*eV, 4.0123*eV, 4.0253*eV, 4.0384*eV, 4.0516*eV, 4.0649*eV, 4.0783*eV, 4.0917*eV, 4.1053*eV, 4.1189*eV, 
                                  4.1327*eV, 4.1465*eV, 4.1604*eV, 4.1744*eV, 4.1885*eV, 4.2027*eV, 4.2170*eV, 4.2314*eV, 4.2459*eV, 4.2605*eV, 
                                  4.2752*eV, 4.2900*eV, 4.3049*eV, 4.3199*eV, 4.3350*eV, 4.3502*eV, 4.3655*eV, 4.3809*eV, 4.3965*eV, 4.4121*eV, 
                                  4.4279*eV, 4.4437*eV, 4.4597*eV, 4.4758*eV, 4.4920*eV, 4.5084*eV, 4.5248*eV, 4.5414*eV, 4.5581*eV, 4.5749*eV, 
                                  4.5919*eV, 4.6089*eV, 4.6261*eV, 4.6434*eV, 4.6609*eV, 4.6785*eV, 4.6962*eV, 4.7141*eV, 4.7321*eV, 4.7502*eV, 
                                  4.7685*eV, 4.7869*eV, 4.8054*eV, 4.8241*eV, 4.8430*eV, 4.8620*eV, 4.8811*eV, 4.9004*eV, 4.9198*eV, 4.9394*eV, 
                                  4.9592*eV};

  std::vector<G4double> transmittance = {0.0101, 0.0101, 0.0103, 0.0103, 0.0103, 0.0103, 0.0104, 0.0104, 0.0107, 0.0107, 
                                        0.0107, 0.0107, 0.0107, 0.0108, 0.0108, 0.0108, 0.0108, 0.0109, 0.0109, 0.0109, 
                                        0.0109, 0.0112, 0.0112, 0.0112, 0.0112, 0.0112, 0.0113, 0.0113, 0.0113, 0.0113, 
                                        0.0115, 0.0115, 0.0115, 0.0115, 0.0115, 0.0116, 0.0116, 0.0116, 0.0116, 0.0117, 
                                        0.0117, 0.0117, 0.0117, 0.0117, 0.0117, 0.0120, 0.0120, 0.0120, 0.0120, 0.0120, 
                                        0.0121, 0.0121, 0.0123, 0.0123, 0.0124, 0.0124, 0.0124, 0.0124, 0.0127, 0.0127, 
                                        0.0127, 0.0128, 0.0128, 0.0129, 0.0129, 0.0130, 0.0130, 0.0132, 0.0132, 0.0132, 
                                        0.0133, 0.0133, 0.0134, 0.0134, 0.0136, 0.0136, 0.0137, 0.0137, 0.0137, 0.0137, 
                                        0.0140, 0.0140, 0.0140, 0.0141, 0.0141, 0.0142, 0.0142, 0.0144, 0.0144, 0.0145, 
                                        0.0145, 0.0146, 0.0146, 0.0146, 0.0149, 0.0149, 0.0150, 0.0150, 0.0152, 0.0152, 
                                        0.0158, 0.0158, 0.0161, 0.0161, 0.0171, 0.0179, 0.0179, 0.0188, 0.0188, 0.0200, 
                                        0.0200, 0.0210, 0.0210, 0.0220, 0.0220, 0.0232, 0.0248, 0.0248, 0.0256, 0.0256, 
                                        0.0273, 0.0273, 0.0286, 0.0286, 0.0296, 0.0296, 0.0311, 0.0311, 0.0324, 0.0339, 
                                        0.0339, 0.0352, 0.0352, 0.0372, 0.0372, 0.0380, 0.0380, 0.0398, 0.0398, 0.0414, 
                                        0.0427, 0.0427, 0.0441, 0.0441, 0.0457, 0.0457, 0.0473, 0.0473, 0.0482, 0.0482, 
                                        0.0492};

  std::vector<G4double> rindex_teflon = {1.3570, 1.3570, 1.3570, 1.3570, 1.3570, 1.3600, 1.3600, 1.3600, 1.3630, 1.3630, 
                                         1.3630, 1.3630, 1.3630, 1.3630, 1.3630, 1.3630, 1.3630, 1.3630, 1.3630, 1.3630, 
                                         1.3630, 1.3660, 1.3660, 1.3630, 1.3630, 1.3660, 1.3660, 1.3660, 1.3710, 1.3710, 
                                         1.3740, 1.3740, 1.3740, 1.3740, 1.3740, 1.3740, 1.3740, 1.3740, 1.3740, 1.3740, 
                                         1.3740, 1.3740, 1.3740, 1.3740, 1.3740, 1.3740, 1.3740, 1.3750, 1.3750, 1.3750, 
                                         1.3760, 1.3760, 1.3790, 1.3790, 1.3820, 1.3820, 1.3820, 1.3820, 1.3820, 1.3820, 
                                         1.3820, 1.3820, 1.3820, 1.3820, 1.3820, 1.3820, 1.3820, 1.3880, 1.3880, 1.3900, 
                                         1.3900, 1.3900, 1.3900, 1.3900, 1.3900, 1.3900, 1.3900, 1.3900, 1.3930, 1.3930, 
                                         1.3930, 1.3930, 1.3930, 1.3940, 1.3940, 1.3940, 1.3960, 1.3960, 1.3960, 1.3980, 
                                         1.3980, 1.3980, 1.3990, 1.3990, 1.3990, 1.4040, 1.4070, 1.4070, 1.4070, 1.4070, 
                                         1.4070, 1.4070, 1.4070, 1.4070, 1.4100, 1.4100, 1.4150, 1.4180, 1.4180, 1.4180, 
                                         1.4180, 1.4180, 1.4180, 1.4180, 1.4180, 1.4180, 1.4180, 1.4200, 1.4230, 1.4230, 
                                         1.4230, 1.4230, 1.4230, 1.4230, 1.4260, 1.4260, 1.4290, 1.4290, 1.4310, 1.4340, 
                                         1.4340, 1.4340, 1.4340, 1.4370, 1.4370, 1.4420, 1.4420, 1.4420, 1.4420, 1.4420, 
                                         1.4420, 1.4420, 1.4450, 1.4450, 1.4480, 1.4480, 1.4510, 1.4510, 1.4510, 1.4510,
                                         1.4530};

  std::vector<G4double> absorption = {1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 
                                      1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um, 1.0000*um,
                                      1.0000*um};       

  // 변수에 값 넣기 

  TEFLON_MPT->AddProperty("RINDEX", energy, rindex_teflon);
  TEFLON_MPT->AddProperty("ABSLENGTH", energy, absorption);

  TeflonInterfaceMpt->AddProperty("TRANSMITTANCE", energy, transmittance);
  TeflonInterfaceMpt->AddProperty("RINDEX", energy, rindex_teflon);

  // 변수를 실제 물질에 적용해주기   
  Mat_Teflon->SetMaterialPropertiesTable(TEFLON_MPT);
  TeflonOpticalSurface->SetMaterialPropertiesTable(TeflonInterfaceMpt);

  // 물에도 동일하게 적용 
  G4MaterialPropertiesTable* Water_MPT = new G4MaterialPropertiesTable();
  G4MaterialPropertiesTable* WaterInterfaceMpt = new G4MaterialPropertiesTable();

  std::vector<G4double> absorption_water = {169.2437*m, 171.3565*m, 173.3922*m, 175.3392*m, 177.1855*m, 178.9191*m, 180.5284*m, 182.0019*m, 183.3286*m, 184.4981*m, 
                                          185.5008*m, 186.3279*m, 186.9718*m, 187.4259*m, 187.6853*m, 187.7459*m, 187.6056*m, 187.2635*m, 186.7204*m, 185.9783*m, 
                                          185.0412*m, 183.9140*m, 182.6033*m, 181.1166*m, 179.4627*m, 177.6512*m, 175.6927*m, 173.5981*m, 171.3789*m, 169.0471*m, 
                                          166.6146*m, 164.0935*m, 161.4955*m, 158.8324*m, 156.1154*m, 153.3555*m, 150.5630*m, 147.7477*m, 144.9188*m, 142.0850*m, 
                                          139.2541*m, 136.4334*m, 133.6296*m, 130.8485*m, 128.0956*m, 125.3755*m, 122.6924*m, 120.0499*m, 117.4511*m, 114.8986*m, 
                                          112.3945*m, 109.9407*m, 107.5384*m, 105.1887*m, 102.8924*m, 100.6498*m, 98.4612*m, 96.3264*m, 94.2454*m, 92.2177*m, 
                                          90.2426*m, 88.3197*m, 86.4480*m, 84.6266*m, 82.8546*m, 81.1310*m, 79.4546*m, 77.8243*m, 76.2389*m, 74.6973*m, 
                                          73.1983*m, 71.7405*m, 70.3228*m, 68.9440*m, 67.6029*m, 66.2981*m, 65.0286*m, 63.7931*m, 62.5905*m, 61.4196*m, 
                                          60.2794*m, 59.1686*m, 58.0863*m, 57.0314*m, 56.0028*m, 54.9996*m, 54.0207*m, 53.0653*m, 52.1324*m, 51.2211*m, 
                                          50.3305*m, 49.4598*m, 48.6082*m, 47.7749*m, 46.9592*m, 46.1602*m, 45.3773*m, 44.6097*m, 43.8569*m, 43.1182*m, 
                                          42.3929*m, 41.6805*m, 40.9804*m, 40.2920*m, 39.6149*m, 38.9486*m, 38.2924*m, 37.6461*m, 37.0092*m, 36.3812*m, 
                                          35.7618*m, 35.1506*m, 34.5473*m, 33.9515*m, 33.3630*m, 32.7814*m, 32.2065*m, 31.6380*m, 31.0758*m, 30.5195*m, 
                                          29.9691*m, 29.4244*m, 28.8852*m, 28.3514*m, 27.8229*m, 27.2996*m, 26.7814*m, 26.2682*m, 25.7600*m, 25.2567*m, 
                                          24.7584*m, 24.2650*m, 23.7765*m, 23.2930*m, 22.8144*m, 22.3408*m, 21.8723*m, 21.4088*m, 20.9505*m, 20.4973*m, 
                                          20.0495*m, 19.6070*m, 19.1699*m, 18.7383*m, 18.3122*m, 17.8918*m, 17.4771*m, 17.0682*m, 16.6652*m, 16.2682*m, 
                                          15.8771*m};

  /* 이건 absortion length의 영향 확인하려고 넣었던 코드
  for (size_t i = 0; i < absorption_water.size(); i++) 
  {
    absorption_water[i] /= 10.0;
  }
  */ 

  std::vector<G4double> rindex_water = {1.3390, 1.3391, 1.3392, 1.3392, 1.3393, 1.3394, 1.3395, 1.3396, 1.3396, 1.3397, 
                                      1.3398, 1.3399, 1.3400, 1.3400, 1.3401, 1.3402, 1.3403, 1.3434, 1.3404, 1.3405, 
                                      1.3406, 1.3407, 1.3408, 1.3408, 1.3409, 1.3410, 1.3411, 1.3412, 1.3412, 1.3413, 
                                      1.3414, 1.3415, 1.3416, 1.3416, 1.3417, 1.3418, 1.3419, 1.3420, 1.3420, 1.3421, 
                                      1.3422, 1.3423, 1.3424, 1.3424, 1.3425, 1.3426, 1.3427, 1.3428, 1.3428, 1.3429, 
                                      1.3430, 1.3431, 1.3432, 1.3434, 1.3435, 1.3436, 1.3437, 1.3438, 1.3440, 1.3441, 
                                      1.3442, 1.3443, 1.3444, 1.3446, 1.3447, 1.3448, 1.3449, 1.3450, 1.3452, 1.3453, 
                                      1.3454, 1.3455, 1.3456, 1.3458, 1.3459, 1.3460, 1.3461, 1.3462, 1.3464, 1.3465, 
                                      1.3466, 1.3467, 1.3468, 1.3470, 1.3471, 1.3472, 1.3473, 1.3474, 1.3476, 1.3477, 
                                      1.3478, 1.3479, 1.3480, 1.3482, 1.3483, 1.3484, 1.3485, 1.3486, 1.3488, 1.3489, 
                                      1.3490, 1.3492, 1.3494, 1.3496, 1.3498, 1.3500, 1.3502, 1.3504, 1.3506, 1.3508, 
                                      1.3510, 1.3512, 1.3514, 1.3516, 1.3518, 1.3520, 1.3522, 1.3524, 1.3526, 1.3528, 
                                      1.3530, 1.3532, 1.3534, 1.3536, 1.3538, 1.3540, 1.3543, 1.3546, 1.3550, 1.3553, 
                                      1.3556, 1.3559, 1.3562, 1.3566, 1.3569, 1.3572, 1.3575, 1.3578, 1.3582, 1.3585, 
                                      1.3588, 1.3591, 1.3594, 1.3598, 1.3601, 1.3604, 1.3607, 1.3610, 1.3614, 1.3617, 
                                      1.3620};
  std::vector<G4double> transmittance_water = {1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 
                                              1.0000};                                      

  Water_MPT->AddProperty("RINDEX", energy, rindex_water);
  Water_MPT->AddProperty("ABSLENGTH", energy, absorption_water);

  WaterInterfaceMpt->AddProperty("TRANSMITTANCE", energy, transmittance_water);
  WaterInterfaceMpt->AddProperty("RINDEX", energy, rindex_water);
 
  Mat_Water->SetMaterialPropertiesTable(Water_MPT);
  TeflonOpticalSurface->SetMaterialPropertiesTable(WaterInterfaceMpt);


  // world 부터 다 보이면 번거로우니까 안보이게 설정
  logical_World->SetVisAttributes(G4VisAttributes::GetInvisible());  
  
  
  return physical_World;

}
