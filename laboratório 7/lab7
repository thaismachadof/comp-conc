//classe da estrutura de dados (recurso) compartilhado entre as threads
class S {
  //recurso compartilhado
  private int r;
  // contador de números pares
  private int pares;
  // vetor utilizado por todas as threads
  private int[] vetor = new int[1000000];

  //construtor
  public S() { 
     this.r = 0;
     this.pares = 0;

    for(int i=0; i<vetor.length; i++)
    {
      vetor[i] = i;
    }
  }

  public synchronized void inc() { 
     this.r++; 
  }

  public synchronized int get() { 
      return this.pares;
  }

  public synchronized void conta_par() { this.pares++; }

  public int get_vetor(int i){ return this.vetor[i]; }

  public int get_tamanho(){ return this.vetor.length; }
}

//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {
   //identificador da thread
   private int id;
   //objeto compartilhado com outras threads
   S s;
   int threads;
  
   //construtor
   public T(int tid, S s, int N) {
      this.id = tid; 
      this.s = s;
      this.threads = N;
   }

   //metodo main da thread
   public void run() {
      System.out.println("Thread " + this.id + " iniciou!");
      for (int i=this.id; i<s.get_tamanho(); i+=threads) {
         if(s.get_vetor(i)%2 == 0)
           this.s.conta_par();
         this.s.inc();  
      }
      System.out.println("Thread " + this.id + " terminou!"); 
   }
}

//classe da aplicacao
class lab7 {
   static final int N = 4;

   public static void main (String[] args) {
      //reserva espaço para um vetor de threads
      Thread[] threads = new Thread[N];

      //cria uma instancia do recurso compartilhado entre as threads
      S s = new S();

      //cria as threads da aplicacao
      for (int i=0; i<threads.length; i++) {
         threads[i] = new T(i, s, N);
      }

      //inicia as threads
      for (int i=0; i<threads.length; i++) {
         threads[i].start();
      }

      //espera pelo termino de todas as threads
      for (int i=0; i<threads.length; i++) {
         try { threads[i].join(); } catch (InterruptedException e) { return; }
      }

      if(s.get() == s.get_tamanho()/2)
         System.out.println("Número de pares = " + s.get());
      else
         System.out.println("ERRO");
   }
}
