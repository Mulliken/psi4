#ifndef SCF_DF_H
#define SCF_DF_H

namespace boost {
template<class T> class shared_ptr;
}

namespace psi {

class BasisSet;
class Matrix;
class Options;
class FittingMetric;
class SchwarzSieve;
class TwoBodyAOInt;
class PSIO;

namespace scf {

class DFHF {

    private:

        // JK J buffers
        // Triangular D matrix
        double* Dtri_;
        // Triangular J matrix
        double* Jtri_;
        // d vector
        double* dQ_;

        // JK K buffers
        // Exchange tensor
        double** Ep_;
        // QS tensor chunk
        double*** QS_;
        // Ctemp matrix
        double*** Ctemp_;
        // Indexing
        int** m_ij_indices_;
        // More indexing
        int** n_indices_;
        // Even more indexing
        int* index_sizes_;

    protected:
        // Constant reference to the options object
        Options& options_;
       
        // Print or not?
        int print_;
        // Is this disk or core?
        bool is_disk_;
        // Is this object RI-J or RI-JK?
        bool is_jk_;
        // Is this object initialized?
        bool is_initialized_;
        // Whether the alpha and beta orbitals are equal or not
        bool restricted_;
        
        // memory in doubles
        unsigned long int memory_;
        
        // Inverse of fitting metric
        boost::shared_ptr<FittingMetric> Jinv_;
        // The three index tensor (or a chunk if on disk)
        boost::shared_ptr<Matrix> Qmn_;

        // AO to USO transform matrix
        boost::shared_ptr<Matrix> AO2USO_;

        // SO Basis quantities
        // Shared pointer to alpha density matrix
        boost::shared_ptr<Matrix> Da_;
        // Shared pointer to beta density matrix
        boost::shared_ptr<Matrix> Db_;
        // Shared pointer to alpha occupation matrix
        boost::shared_ptr<Matrix> Ca_;
        // Shared pointer to beta occupation matrix
        boost::shared_ptr<Matrix> Cb_;
        // Shared pointer to total Coulomb matrix
        boost::shared_ptr<Matrix> Ja_;
        // Shared pointer to alpha Exchange matrix
        boost::shared_ptr<Matrix> Ka_;
        // Shared pointer to beta Exchange matrix
        boost::shared_ptr<Matrix> Kb_;
        // Number of alpha electrons
        const int* nalphapi_;
        // Number of beta electrons
        const int* nbetapi_;
       
        // AO Basis quantities
        // Shared pointer to alpha density matrix
        boost::shared_ptr<Matrix> Da_ao_;
        // Shared pointer to beta density matrix
        boost::shared_ptr<Matrix> Db_ao_;
        // Shared pointer to alpha occupation matrix
        boost::shared_ptr<Matrix> Ca_ao_;
        // Shared pointer to beta occupation matrix
        boost::shared_ptr<Matrix> Cb_ao_;
        // Shared pointer to total Coulomb matrix
        boost::shared_ptr<Matrix> Ja_ao_;
        // Shared pointer to alpha Exchange matrix
        boost::shared_ptr<Matrix> Ka_ao_;
        // Shared pointer to beta Exchange matrix
        boost::shared_ptr<Matrix> Kb_ao_;
        // Number of alpha electrons
        int nalpha_;
        // Number of beta electrons
        int nbeta_;
 
        // Zero basis set
        boost::shared_ptr<BasisSet> zero_;
        // Primary basis set
        boost::shared_ptr<BasisSet> primary_;
        // Auxiliary basis set
        boost::shared_ptr<BasisSet> auxiliary_;
        // Schwarz Sieve object
        boost::shared_ptr<SchwarzSieve> schwarz_;

        // The psio object
        boost::shared_ptr<PSIO> psio_;
        // Is the object disk initialized?
        bool is_initialized_disk_;
        // Buffer A for AIO
        boost::shared_ptr<Matrix> QmnA_;
        // Buffer B for AIO
        boost::shared_ptr<Matrix> QmnB_;
        // Use A or B buffer for computation
        bool aio_bufferA_;
        // Current iteration disk order
        bool aio_forward_;

        // Setup
        void common_init();
        // Initialize methods
        void initialize();
        void initialize_JK_disk();
        void initialize_JK_core();
        void initialize_J_core();

        // Fast core J
        void compute_J_core();
        // Block methods
        void compute_JK_block_J(double** Qmnp, int nrows, int max_rows);
        void compute_JK_block_K(double** Qmnp, int nrows, int max_rows, bool is_alpha);

        // Setup the memory for USO2AO stuff
        void initializeUSO2AO();
        // Transform C/D back to AO
        void USO2AO();
        // Transform J/K forward to USO
        void AO2USO();

    public:
        // Constructor for generic HF, with J, K, D to be set later
        DFHF(boost::shared_ptr<BasisSet> basis, boost::shared_ptr<PSIO> psio, Options& opt);
        // Destructor
        ~DFHF();

        // Setter methods, to be called from the JK functors
        void set_jk(bool jk) { is_jk_ = jk; }
        void set_restricted(bool y_n) { restricted_ = y_n; }
        void set_J(boost::shared_ptr<Matrix> Ja) {Ja_ = Ja;}
        void set_Ka(boost::shared_ptr<Matrix> Ka) {Ka_ = Ka;}
        void set_Kb(boost::shared_ptr<Matrix> Kb) {Kb_ = Kb;}
        void set_Da(boost::shared_ptr<Matrix> Da) {Da_ = Da;}
        void set_Db(boost::shared_ptr<Matrix> Db) {Db_ = Db;}
        void set_Ca(boost::shared_ptr<Matrix> Ca) {Ca_ = Ca;}
        void set_Cb(boost::shared_ptr<Matrix> Cb) {Cb_ = Cb;}
        void set_Na(const int* Na) {nalphapi_ = Na;}
        void set_Nb(const int* Nb) {nbetapi_ = Nb;}

        // form J only
        void form_J_DF();
        // form J and K
        void form_JK_DF();

};

}}

#endif
