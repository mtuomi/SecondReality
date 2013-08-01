uses crt,aos1,aos2,aos3,bgr;

const  veke = 2800;

procedure setrgb(c,r,g,b:byte);
begin
     port[$3c8] := c;
     port[$3c9] := r;
     port[$3c9] := g;
     port[$3c9] := b;
end;

var  frames : word;
      w     : word;


procedure waitr;
begin
        asm
	mov	bx,1
	int	0fch
        mov     frames,ax
        end;
end;


function dis_exit:boolean;
var   a : byte;
begin
        asm
	mov	bx,2
	int	0fch
        mov     a,al
        end;
        if a = 0 then dis_exit := false else dis_exit := true;
end;

var   f : file;
      inx : word;
      y,x,t   : word;
      pal    : array[0..768] of byte;
      font   : array[0..320*31] of byte;
      fpal,tmppal : array[0..768] of byte;
      scp,sss:word;

{$L routines.obj}
          { dx = Bground seg,
            ax = pos seg,
            si = pos ofs,
            cx = font seg,
            bx = font ofs }

procedure putrouts;far;external;

procedure scr(pos:byte);
begin
  case pos of

   0:asm
      mov   dx,seg hback
      mov   ax,seg posi1
      mov   si,offset posi1
      mov   cx,seg font
      mov   bx,offset font
      add   bx,scp
      call  putrouts
     end;
   1:asm
      mov   dx,seg hback
      mov   ax,seg posi2
      mov   si,offset posi2
      mov   cx,seg font
      mov   bx,offset font
      add   bx,scp
      call  putrouts
     end;
   2:asm
      mov   dx,seg hback
      mov   ax,seg posi3
      mov   si,offset posi3
      mov   cx,seg font
      mov   bx,offset font
      add   bx,scp
      call  putrouts
      inc   scp
     end;
  end;
end;


var   frame : word;
      quit  : word;
      ch    : char;
      perse : boolean;
      fadeout : boolean;
      fp      : byte;
begin
     asm
     mov   ax,$13
     int   $10

     xor   bx,bx
     int   0fch

     end;

     assign(f,'o.sci');
     reset(f,1);
     for x := 0 to 30 do
      begin
       seek(f,x*320+778);
       blockread(f,font[x*237],237);
      end;
     close(f);

     for x := 0 to sizeof(font) do if font[x] > 0 then inc(font[x],128);
     move(mem[seg(hback):ofs(hback)+10],pal,768);
     move(mem[seg(hback):ofs(hback)+778], mem[seg(hback):ofs(hback)], 64000);
     for x := 0 to 255 do setrgb(x,0,0,0);
     move(mem[seg(hback):ofs(hback)],mem[$a000:0],64000);

     move(pal,tmppal,768);
     fillchar(tmppal,32*3,#0);
     fillchar(tmppal[128*3],32*3,#0);
     fillchar(fpal,768,#0);

     w := 1;     { Wait raster routine }

     for y := 0 to 63 do
      begin
        waitr;
        for x := 0 to 255 do setrgb(x,fpal[x*3+0],fpal[x*3+1],fpal[x*3+2]);
        for x := 0 to 255 do
         begin
          if fpal[x*3+0] < tmppal[x*3+0] then inc(fpal[x*3+0]);
          if fpal[x*3+1] < tmppal[x*3+1] then inc(fpal[x*3+1]);
          if fpal[x*3+2] < tmppal[x*3+2] then inc(fpal[x*3+2]);
         end;
      end;

     move(pal,tmppal,768);
     move(pal,fpal,768);
     fillchar(fpal,32*3,#0);
     fillchar(fpal[128*3],32*3,#0);
     for x := 0 to 255 do setrgb(x,fpal[x*3+0],fpal[x*3+1],fpal[x*3+2]);

     asm
@a:  mov   ax,0
     mov   bx,6
     int   0fch
     cmp   dx,0
     jl    @a
     end;

     for y := 0 to 150 do waitr;

     sss := 0;
     scp := 0;

     for y := 0 to 63*2 do
      begin
        waitr;
        scr(sss);
        if sss = 2 then sss := 0 else inc(sss);

        if y and 1 = 1 then
        begin
        for x := 0 to 176 do setrgb(x,fpal[x*3+0],fpal[x*3+1],fpal[x*3+2]);
        for x := 0 to 176 do
         begin
          if fpal[x*3+0] < tmppal[x*3+0] then inc(fpal[x*3+0]);
          if fpal[x*3+1] < tmppal[x*3+1] then inc(fpal[x*3+1]);
          if fpal[x*3+2] < tmppal[x*3+2] then inc(fpal[x*3+2]);
         end;
        end;
      end;

    fillchar(tmppal,768,#0);


    { Loppu looppi }

    frame := 0;
    w     := 0;
    ch    := #0;
    quit  := 0;
    repeat
     waitr;
     scr(sss);
     if sss = 2 then sss := 0 else inc(sss);

{     inc(frame);}

     asm
     mov   ax,0
     mov   bx,6
     int   0fch
     cmp   dx,-11
     jnz   @a
     mov   fadeout,1
@a:  end;



      if fadeout then
        begin
        if fp = 64 then quit := 1 else inc(fp);
        for x := 0 to 255 do
         begin
          setrgb(x,fpal[x*3+0],fpal[x*3+1],fpal[x*3+2]);
          if fpal[x*3+0] > tmppal[x*3+0] then dec(fpal[x*3+0]);
          if fpal[x*3+1] > tmppal[x*3+1] then dec(fpal[x*3+1]);
          if fpal[x*3+2] > tmppal[x*3+2] then dec(fpal[x*3+2]);
         end;
        end;

     until dis_exit or (frame = veke) or (quit = 1);
end.