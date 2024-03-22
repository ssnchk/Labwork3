#include "SandParse.h"
Args Parse(int argc, char *argv[]) {
    Args args{};
    std::ifstream input_File;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i],"-i")==0 || strcmp(argv[i],"--input")==0) {
            args.input = argv[i+1];
        }
        if (strcmp(argv[i],"-o")==0 || strcmp(argv[i],"--output")==0) {
            args.output = argv[i+1];
        }
        if (strcmp(argv[i],"-m")==0 || strcmp(argv[i],"--max-iter")==0) {
            args.max_iter = std::stoll(argv[i+1]);
        }
        if (strcmp(argv[i],"-f")==0 || strcmp(argv[i],"--freq")==0) {
            args.freq = std::stoll(argv[i+1]);
        }
    }
    input_File.open(args.input);
    if (!input_File.is_open()){
        std::cerr<< "Wrong input argument";
        exit(1);
    }
    input_File.close();
    return args;
}

tsv_args Tsv_Parse(const char* path){
    std::ifstream File(path);
    int16_t x;
    int16_t y;
    uint64_t val;
    uint16_t file_length=0;
    while (!File.eof()){
        File>>x;
        ++file_length;
    }
    tsv_args tsv{file_length};
    File.clear();
    File.seekg(0);
    for (uint16_t i=0;i<file_length/3;++i) {
        File >> x >> y >> val;
        tsv.max_x=std::max(tsv.max_x,x);
        tsv.max_y=std::max(tsv.max_y,y);
        tsv.min_x=std::min(tsv.min_x,x);
        tsv.min_y=std::min(tsv.min_y,y);
        tsv.x_coord[i]=x;
        tsv.y_coord[i]=y;
        tsv.sand_val[i]=val;
    }
    File.close();
    return tsv;
}

Field tsv_to_field(tsv_args& tsv) {
    Field pole{static_cast<uint32_t>(tsv.max_y - tsv.min_y)+1, static_cast<uint32_t>(tsv.max_x - tsv.min_x)+1};
    for (uint16_t i = 0; i < tsv.args_amount; ++i) {
        if (tsv.sand_val[i]>=4){
            pole.unstable.push_forward(tsv.x_coord[i]- tsv.min_x,tsv.y_coord[i]- tsv.min_y,tsv.sand_val[i]);
        }
        pole.m_field[(tsv.y_coord[i]- tsv.min_y)*pole.width+tsv.x_coord[i]- tsv.min_x]=tsv.sand_val[i];
    }
    return pole;
}

